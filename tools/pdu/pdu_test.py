#! /usr/bin/env python3

from pyftdi import spi
from pyftdi.ftdi import Ftdi
Ftdi.show_devices()

sp = spi.SpiController()
sp.configure('ftdi://::/1')
slave = sp.get_port(cs=0, freq=1e6, mode=2)
write_buf  = b'\b00001000\x00'
slave.write(write_buf)
