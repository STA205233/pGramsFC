#!/usr/bin/env python3
"""
VS-30-C-2-3 Inclinometer Emulator
Level Developments Ltd. - LD Standard Communication Protocol (Binary Mode)

[Command specification]
  - Commands are 7 printable ASCII bytes (no line ending, no padding)
  - Case-insensitive
  - Inter-character timeout: 100 ms (per datasheet); buffer is flushed on timeout

[Binary response format] (no line endings)
  Command   | Response bytes | Format
  ----------+----------------+------------------------------------------
  get-x&y   |  8 bytes       | INT32(X*1000) + INT32(Y*1000)  Big Endian
  gettemp   |  2 bytes       | INT16(temp*100)                Big Endian
  unknown   |  2 bytes       | 0xFF 0xFF

  Example: X=+2.351 deg, Y=-1.205 deg
    INT32( 2351) = 00 00 09 2F
    INT32(-1205) = FF FF FB 4B
    -> TX: 00 00 09 2F FF FF FB 4B

  Example: temp=+25.03 C
    INT16(2503) = 09 C7
    -> TX: 09 C7

[Usage]
  Linux/macOS - create a virtual port pair:
    socat -d -d pty,raw,echo=0,link=/tmp/ttyVS30 pty,raw,echo=0,link=/tmp/ttyClient

  Start emulator:
    python vs30_emulator.py --port /tmp/ttyVS30

  Windows (after creating a pair with com0com):
    python vs30_emulator.py --port COM10
"""

import struct
import serial
import time
import math
import random
import argparse
import sys
from datetime import datetime

# -----------------------------------------------
# Constants
# -----------------------------------------------
DEFAULT_PORT      = "/dev/ttyS1"
DEFAULT_BAUDRATE  = 1200
COMMAND_LENGTH    = 7       # commands are exactly 7 ASCII bytes
CHAR_TIMEOUT_SEC  = 0.100   # inter-character timeout (100 ms, per datasheet)


# -----------------------------------------------
# Sensor simulator
# -----------------------------------------------
class SensorState:
    def __init__(
        self,
        x_center:    float = 0.0,
        y_center:    float = 0.0,
        x_amp:       float = 2.5,
        y_amp:       float = 1.8,
        x_period:    float = 20.0,
        y_period:    float = 15.0,
        temp_center: float = 25.0,
        noise_deg:   float = 0.002,
    ):
        self.x_center    = x_center
        self.y_center    = y_center
        self.x_amp       = x_amp
        self.y_amp       = y_amp
        self.x_period    = x_period
        self.y_period    = y_period
        self.temp_center = temp_center
        self.noise_deg   = noise_deg
        self._t0         = time.time()

    def _t(self) -> float:
        return time.time() - self._t0

    def angle_x(self) -> float:
        v = self.x_center + self.x_amp * math.sin(2 * math.pi * self._t() / self.x_period)
        return v + random.uniform(-self.noise_deg, self.noise_deg)

    def angle_y(self) -> float:
        v = self.y_center + self.y_amp * math.cos(2 * math.pi * self._t() / self.y_period)
        return v + random.uniform(-self.noise_deg, self.noise_deg)

    def temperature(self) -> float:
        v = self.temp_center + 0.3 * math.sin(2 * math.pi * self._t() / 60.0)
        return v + random.uniform(-0.005, 0.005)


# -----------------------------------------------
# Binary encoders
# -----------------------------------------------
def encode_int32(value: float) -> bytes:
    """Angle [deg] -> INT32 Big Endian (resolution 0.001 deg)"""
    raw = int(round(value * 1000))
    raw = max(-2_147_483_648, min(2_147_483_647, raw))
    return struct.pack(">i", raw)

def encode_int16(value: float) -> bytes:
    """Temperature [C] -> INT16 Big Endian (resolution 0.01 C)"""
    raw = int(round(value * 100))
    raw = max(-32768, min(32767, raw))
    return struct.pack(">h", raw)


# -----------------------------------------------
# Command dispatcher
# -----------------------------------------------
def dispatch(cmd_bytes: bytes, sensor: SensorState) -> bytes:
    """Receive a command bytes and return a binary response."""
    cmd = cmd_bytes.decode("ascii", errors="ignore").strip().lower()

    if cmd == "get-x&y":
        # 8 bytes: INT32(X*1000) + INT32(Y*1000)  Big Endian
        return encode_int32(sensor.angle_x()) + encode_int32(sensor.angle_y())

    if cmd == "gettemp":
        # 2 bytes: INT16(temp*100)  Big Endian
        return encode_int16(sensor.temperature())

    # Unknown command
    return b"\xff\xff"


# -----------------------------------------------
# Command receive buffer
#
# Strategy:
#   - Accumulate incoming bytes one by one.
#   - Dispatch immediately when COMMAND_LENGTH (7) bytes are buffered.
#   - Also dispatch whatever is buffered when the inter-character gap
#     exceeds CHAR_TIMEOUT_SEC (handles short/truncated transmissions).
#
# This makes the buffer robust against any chunk size the OS delivers,
# whether bytes arrive all at once or split across multiple reads.
# -----------------------------------------------
class CommandBuffer:
    def __init__(self):
        self._buf: bytes    = b""
        self._last_rx: float = 0.0

    def feed(self, data: bytes) -> list:
        """
        Feed received bytes and return a list of completed commands.
        Each entry is the raw bytes that form one command.
        """
        now = time.monotonic()
        completed = []

        for byte in data:
            gap = now - self._last_rx

            # Inter-character timeout: flush whatever is buffered as a command
            if self._buf and gap > CHAR_TIMEOUT_SEC:
                completed.append(self._buf)
                self._buf = b""

            self._buf    += bytes([byte])
            self._last_rx = now

            # Exact length match: dispatch immediately
            if len(self._buf) == COMMAND_LENGTH:
                completed.append(self._buf)
                self._buf = b""

        return completed

    def flush_on_timeout(self) -> list:
        """
        Call this periodically (e.g. every read loop iteration) to flush
        a partial buffer that has been sitting idle longer than the timeout.
        Returns a list of commands (0 or 1 entries).
        """
        if not self._buf:
            return []
        if (time.monotonic() - self._last_rx) > CHAR_TIMEOUT_SEC:
            cmd = self._buf
            self._buf = b""
            return [cmd]
        return []


# -----------------------------------------------
# Logging
# -----------------------------------------------
def ts() -> str:
    return datetime.now().strftime("%H:%M:%S.%f")[:-3]

def log_rx(raw: bytes):
    cmd_str = raw.decode("ascii", errors="replace").strip()
    print(f"[{ts()}] -> RX  {raw.hex(' ')}  ({cmd_str!r})")

def log_tx(raw: bytes):
    n = len(raw)
    if n == 8:
        x = struct.unpack(">i", raw[0:4])[0]
        y = struct.unpack(">i", raw[4:8])[0]
        detail = f"X={x/1000:+.3f} deg  Y={y/1000:+.3f} deg"
    elif n == 2 and raw != b"\xff\xff":
        t = struct.unpack(">h", raw)[0]
        detail = f"temp={t/100:+.2f} C"
    else:
        detail = "ERROR"
    print(f"[{ts()}] <- TX  {raw.hex(' ')}  ({detail})")


# -----------------------------------------------
# Main loop
# -----------------------------------------------
def run_emulator(port: str, baudrate: int, verbose: bool, sensor: SensorState):
    print("=" * 62)
    print("  VS-30-C-2-3 Inclinometer Emulator  [Binary Mode]")
    print("  Level Developments - LD Standard Protocol")
    print("=" * 62)
    print(f"  Port          : {port}")
    print(f"  Baud rate     : {baudrate} baud, 8N1")
    print(f"  Command length: {COMMAND_LENGTH} bytes (no line ending)")
    print(f"  Char timeout  : {int(CHAR_TIMEOUT_SEC * 1000)} ms")
    print(f"  get-x&y reply : 8 bytes  INT32(X*1000) + INT32(Y*1000) Big Endian")
    print(f"  gettemp reply : 2 bytes  INT16(temp*100) Big Endian")
    print("  Press Ctrl+C to quit")
    print("=" * 62)

    try:
        ser = serial.Serial(
            port=port,
            baudrate=baudrate,
            bytesize=serial.EIGHTBITS,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            timeout=0.05,       # short read timeout so flush_on_timeout() runs regularly
        )
    except serial.SerialException as e:
        print(f"\n[ERROR] Could not open port: {e}")
        print("  Linux/macOS:")
        print("    socat -d -d pty,raw,echo=0,link=/tmp/ttyVS30 "
              "pty,raw,echo=0,link=/tmp/ttyClient")
        sys.exit(1)

    cmd_buf = CommandBuffer()

    def handle_commands(commands: list):
        for cmd in commands:
            if verbose:
                log_rx(cmd)
            response = dispatch(cmd, sensor)
            time.sleep(0.002)   # simulate device response latency (~2 ms)
            ser.write(response)
            if verbose:
                log_tx(response)

    try:
        while True:
            chunk = ser.read(64)

            if chunk:
                print(chunk)
                handle_commands(cmd_buf.feed(chunk))

            # Flush any partial command that has been idle past the timeout
            handle_commands(cmd_buf.flush_on_timeout())

    except KeyboardInterrupt:
        print("\n[INFO] Emulator stopped.")
    finally:
        ser.close()


# -----------------------------------------------
# CLI
# -----------------------------------------------
def main():
    parser = argparse.ArgumentParser(
        description="VS-30-C-2-3 Inclinometer Emulator (Binary, 7-byte commands)"
    )
    parser.add_argument("-p", "--port",
        default=DEFAULT_PORT,
        help=f"Serial port (default: {DEFAULT_PORT})")
    parser.add_argument("-b", "--baudrate",
        type=int, default=DEFAULT_BAUDRATE,
        help=f"Baud rate (default: {DEFAULT_BAUDRATE})")
    parser.add_argument("--x-center",
        type=float, default=0.0, metavar="DEG",
        help="X axis resting angle in degrees (default: 0.0)")
    parser.add_argument("--y-center",
        type=float, default=0.0, metavar="DEG",
        help="Y axis resting angle in degrees (default: 0.0)")
    parser.add_argument("--x-amp",
        type=float, default=2.5, metavar="DEG",
        help="X axis oscillation amplitude in degrees (default: 2.5)")
    parser.add_argument("--y-amp",
        type=float, default=1.8, metavar="DEG",
        help="Y axis oscillation amplitude in degrees (default: 1.8)")
    parser.add_argument("--temp",
        type=float, default=25.0, metavar="CEL",
        help="Base temperature in Celsius (default: 25.0)")
    parser.add_argument("--noise",
        type=float, default=0.002, metavar="DEG",
        help="Angle noise amplitude in degrees (default: 0.002)")
    parser.add_argument("-q", "--quiet",
        action="store_true",
        help="Suppress log output")

    args = parser.parse_args()

    sensor = SensorState(
        x_center=args.x_center,
        y_center=args.y_center,
        x_amp=args.x_amp,
        y_amp=args.y_amp,
        temp_center=args.temp,
        noise_deg=args.noise,
    )

    run_emulator(
        port=args.port,
        baudrate=args.baudrate,
        verbose=not args.quiet,
        sensor=sensor,
    )


if __name__ == "__main__":
    main()