#!/usr/bin/env python3
"""
SiPM bias control board simulator.

Emulates the serial-port behavior of the SiPM bias board described by the
board firmware developer, so that the ToFBiasController client code
(onboard/source/core/src/ToFBiasController.cc) can be exercised without the
real hardware.

Command syntax: "<cmd> [ch] [val]", one command per line.

  data [val]            val = on|off   (default: off)         start/stop the 1 Hz data stream
  denable [ch] [val]    ch = all|0-7   val = on|off (default: off)   enable/disable DC-DC converters
  tenable [val]         val = on|off   (default: on)           enable/disable temperature control
  temp [ch] [val]       ch = 0-15      val = 218-423 (K, default: 298)   default temperature
  trim [ch] [val]       ch = 0-127     val = 140-172 (default: 164)      default trim voltage
  p                     dump current board state as one line terminated by '\n'

Data frame (sent once per second while streaming is on):
  128 ADC values  (4 hex chars each) = 512 chars
  32  temp values (4 hex chars each) = 128 chars
  128 DAC values  (3 hex chars each) = 384 chars
  total: 1024 ASCII chars, no separators, no trailing newline.

Usage:
  python3 board_simulator.py /dev/ttyXXXX
  python3 board_simulator.py --pty      # creates a pty pair for local testing
"""
import argparse
import os
import pty
import select
import sys
import threading
import time
import tty

NUM_ADC = 128
NUM_TEMP = 32
NUM_DAC = 128

NUM_DENABLE_CH = 8
NUM_TEMP_CH = 16
NUM_TRIM_CH = 128

TEMP_MIN, TEMP_MAX, TEMP_DEFAULT = 218, 423, 298
TRIM_MIN, TRIM_MAX, TRIM_DEFAULT = 140, 172, 164


class BoardState:
    def __init__(self):
        self.data_stream_on = False
        self.denable = [False] * NUM_DENABLE_CH
        self.tenable = True
        self.temp = [TEMP_DEFAULT] * NUM_TEMP_CH
        self.trim = [TRIM_DEFAULT] * NUM_TRIM_CH

    def summary(self):
        return (
            f"data={'on' if self.data_stream_on else 'off'} "
            f"denable={['on' if v else 'off' for v in self.denable]} "
            f"tenable={'on' if self.tenable else 'off'} "
            f"temp={self.temp} "
            f"trim={self.trim}"
        )


def parse_channels(ch_toks, num_channels):
    """Parse space-separated channel tokens ("all" or one-or-more ints)."""
    if len(ch_toks) == 1 and ch_toks[0].lower() == "all":
        return list(range(num_channels))
    return [int(c) for c in ch_toks]


def parse_onoff(tok):
    if tok is None:
        return None
    tok = tok.lower()
    if tok == "on":
        return True
    if tok == "off":
        return False
    return None


class BoardSimulator:
    def __init__(self, fd, state: BoardState, debug: bool = False):
        self.fd = fd
        self.state = state
        self.debug = debug
        self.lock = threading.Lock()
        self.stop_event = threading.Event()

    def write(self, s: str):
        os.write(self.fd, s.encode("ascii"))

    def log(self, msg: str):
        print(f"[board] {msg}", flush=True)

    def debug_log(self, msg: str):
        if self.debug:
            print(f"[board][debug] {msg}", flush=True)

    def handle_line(self, line: str):
        self.debug_log(f"received: {line!r}")
        line = line.strip()
        if not line:
            return
        parts = line.split()
        cmd = parts[0].lower()
        args = parts[1:]

        with self.lock:
            if cmd == "data":
                val = parse_onoff(args[0]) if args else False
                if val is None:
                    self.log(f"data: invalid value '{args[0]}'")
                    return
                self.state.data_stream_on = val
                self.log(f"data stream turned {'on' if val else 'off'}")

            elif cmd == "denable":
                if len(args) < 2:
                    self.log(f"denable: missing arguments in '{line}'")
                    return
                ch_toks, val_tok = args[:-1], args[-1]
                val = parse_onoff(val_tok)
                if val is None:
                    self.log(f"denable: invalid value '{val_tok}'")
                    return
                try:
                    channels = parse_channels(ch_toks, NUM_DENABLE_CH)
                except ValueError:
                    self.log(f"denable: invalid channel(s) '{' '.join(ch_toks)}'")
                    return
                for ch in channels:
                    if not (0 <= ch < NUM_DENABLE_CH):
                        self.log(f"denable: channel {ch} out of range (0-{NUM_DENABLE_CH - 1})")
                        continue
                    self.state.denable[ch] = val
                self.log(f"denable channel(s) {channels} turned {'on' if val else 'off'}")

            elif cmd == "tenable":
                val = parse_onoff(args[0]) if args else True
                if val is None:
                    self.log(f"tenable: invalid value '{args[0]}'")
                    return
                self.state.tenable = val
                self.log(f"temperature control turned {'on' if val else 'off'}")

            elif cmd == "temp":
                if len(args) < 2:
                    self.log(f"temp: missing arguments in '{line}'")
                    return
                ch_toks, val_tok = args[:-1], args[-1]
                try:
                    val = int(val_tok)
                except ValueError:
                    self.log(f"temp: invalid value '{val_tok}'")
                    return
                try:
                    channels = parse_channels(ch_toks, NUM_TEMP_CH)
                except ValueError:
                    self.log(f"temp: invalid channel(s) '{' '.join(ch_toks)}'")
                    return
                if not (TEMP_MIN <= val <= TEMP_MAX):
                    self.log(f"temp: value {val} out of range ({TEMP_MIN}-{TEMP_MAX})")
                    return
                for ch in channels:
                    if not (0 <= ch < NUM_TEMP_CH):
                        self.log(f"temp: channel {ch} out of range (0-{NUM_TEMP_CH - 1})")
                        continue
                    self.state.temp[ch] = val
                self.log(f"temp channel(s) {channels} set to {val} K")

            elif cmd == "trim":
                if len(args) < 2:
                    self.log(f"trim: missing arguments in '{line}'")
                    return
                ch_toks, val_tok = args[:-1], args[-1]
                try:
                    val = int(val_tok)
                except ValueError:
                    self.log(f"trim: invalid value '{val_tok}'")
                    return
                try:
                    channels = parse_channels(ch_toks, NUM_TRIM_CH)
                except ValueError:
                    self.log(f"trim: invalid channel(s) '{' '.join(ch_toks)}'")
                    return
                if not (TRIM_MIN <= val <= TRIM_MAX):
                    self.log(f"trim: value {val} out of range ({TRIM_MIN}-{TRIM_MAX})")
                    return
                for ch in channels:
                    if not (0 <= ch < NUM_TRIM_CH):
                        self.log(f"trim: channel {ch} out of range (0-{NUM_TRIM_CH - 1})")
                        continue
                    self.state.trim[ch] = val
                self.log(f"trim channel(s) {channels} set to {val}")

            elif cmd == "p":
                self.log(f"print requested: {self.state.summary()}")
                self.write(self.state.summary() + "\n")

            else:
                self.log(f"unknown command '{line}'")

    def build_frame(self) -> str:
        with self.lock:
            adc_vals = []
            for ch in range(NUM_ADC):
                enabled = self.state.denable[ch % NUM_DENABLE_CH]
                adc_vals.append(0x1000 + ch if enabled else 0)

            temp_vals = []
            for ch in range(NUM_TEMP):
                if self.state.tenable:
                    t = self.state.temp[ch % NUM_TEMP_CH]
                else:
                    t = 0
                temp_vals.append(t & 0xFFFF)

            dac_vals = [self.state.trim[ch] & 0xFFF for ch in range(NUM_DAC)]

        parts = []
        parts.extend(f"{v:04X}" for v in adc_vals)
        parts.extend(f"{v:04X}" for v in temp_vals)
        parts.extend(f"{v:03X}" for v in dac_vals)
        frame = "".join(parts)
        assert len(frame) == NUM_ADC * 4 + NUM_TEMP * 4 + NUM_DAC * 3
        return frame

    def stream_loop(self):
        next_tick = time.monotonic()
        while not self.stop_event.is_set():
            next_tick += 1.0
            with self.lock:
                streaming = self.state.data_stream_on
            if streaming:
                frame = self.build_frame()
                try:
                    self.write(frame)
                except OSError:
                    break
                self.log(f"sent data frame ({len(frame)} chars)")
            sleep_time = next_tick - time.monotonic()
            if sleep_time > 0:
                self.stop_event.wait(sleep_time)
            else:
                next_tick = time.monotonic()

    def read_loop(self):
        # WriteData() on the client side does not append a trailing
        # newline, so a command's end can only be recognized by a
        # newline OR by a pause in incoming bytes (idle timeout). Use
        # select() so a partial buffer gets flushed once no more data
        # shows up within IDLE_FLUSH_SEC.
        IDLE_FLUSH_SEC = 0.05
        buf = b""
        while not self.stop_event.is_set():
            ready, _, _ = select.select([self.fd], [], [], IDLE_FLUSH_SEC)
            if not ready:
                if buf:
                    line, buf = buf, b""
                    self.handle_line(line.decode("ascii", errors="replace"))
                continue
            try:
                chunk = os.read(self.fd, 1024)
            except OSError:
                break
            if not chunk:
                break
            self.debug_log(f"raw chunk: {chunk!r}")
            buf += chunk
            while b"\n" in buf:
                line, buf = buf.split(b"\n", 1)
                self.handle_line(line.decode("ascii", errors="replace"))

    def run(self):
        t_stream = threading.Thread(target=self.stream_loop, daemon=True)
        t_read = threading.Thread(target=self.read_loop, daemon=True)
        t_stream.start()
        t_read.start()
        try:
            while t_read.is_alive():
                t_read.join(timeout=0.5)
        except KeyboardInterrupt:
            pass
        finally:
            self.stop_event.set()


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("port", nargs="?", help="serial device to open (e.g. /dev/ttyACM0)")
    parser.add_argument("--pty", action="store_true", help="create a pty pair instead of opening a real serial device")
    parser.add_argument("--baudrate", type=int, default=115200)
    parser.add_argument("--debug", action="store_true", help="print raw received bytes/lines in addition to interpreted actions")
    args = parser.parse_args()

    state = BoardState()

    if args.pty:
        master_fd, slave_fd = pty.openpty()
        slave_name = os.ttyname(slave_fd)
        # Put the slave side in raw mode: otherwise the tty line discipline
        # (canonical mode, echo) mangles/echoes bytes written by the board
        # (e.g. long data frames overflow the canonical input queue and
        # trigger BEL characters), which the read loop then misinterprets
        # as incoming commands.
        tty.setraw(slave_fd)
        print(f"[board] pty created, client should connect to: {slave_name}")
        fd = master_fd
    elif args.port:
        try:
            import serial
        except ImportError:
            print("pyserial is required to open a real serial device.\n"
                  "Install it with: pip install pyserial\n"
                  "Alternatively, run with --pty to test without hardware.", file=sys.stderr)
            sys.exit(1)
        ser = serial.Serial(args.port, args.baudrate, timeout=None)
        fd = ser.fileno()
    else:
        parser.error("either a serial PORT or --pty must be given")

    print(f"[board] simulator started ({'pty' if args.pty else args.port} @ {args.baudrate} bps)")
    sim = BoardSimulator(fd, state, debug=args.debug)
    sim.run()


if __name__ == "__main__":
    main()
