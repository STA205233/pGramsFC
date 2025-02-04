#! /usr/bin/env python3
import socket
import sys


def main(host, port):
    # Create a socket object
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    s.bind((socket.gethostname(), port))
    try:
        s.listen(5)
        clientsocket, address = s.accept()
        print(f"Connection from {address} has been established!")
    except KeyboardInterrupt:
        s.close()
        sys.exit(0)
    try:
        while (True):
            # Receive no more than 1024 bytes
            msg = clientsocket.recv(1024)
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
