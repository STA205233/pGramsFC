#! /usr/bin/env python3
import socket
import sys


def main(host, port):
    # Create a socket object
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Connection to hostname on the port.
    s.connect((host, port))
    try:
        while (True):
            # Receive no more than 1024 bytes
            msg = s.recv(1024)
            print(f"Received: {len(msg)} bytes")
            for byte in msg:
                print(f"{hex(byte)}", end=" ")
    except KeyboardInterrupt:
        pass
    s.close()


if __name__ == '__main__':
    if (len(sys.argv) != 3):
        print(f"Usage: {sys.argv[0]} <host> <port>")
        sys.exit(1)
    main(sys.argv[1], int(sys.argv[2]))
