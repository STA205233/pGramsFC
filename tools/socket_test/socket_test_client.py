#! /usr/bin/env python3
import socket
import sys
import time
if __name__ == "__main__":
    if (len(sys.argv) != 3):
        print(f"Usage: {sys.argv[0]} <ip> <port>")
        sys.exit(1)
    host = sys.argv[1]
    port = int(sys.argv[2])
    address = (host, port)
    # Create a socket object

    while True:
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            print(f"Connectiong on {host}:{port}")
            s.connect(address)
            print(f"Connection from {address[0]} has been established!")

        except KeyboardInterrupt:
            s.close()
            sys.exit(0)
        except TimeoutError:
            print("Timeout")
            s.close()
            del s
            continue
        try:
            for i in range(1):
                # Receive no more than 1024 bytes
                s.send("Hello, World!".encode())
                print()
                msg = s.recv(1024)
                if len(msg) == 0:
                    break
                print(f"Received: {len(msg)} bytes")
                for byte in msg:
                    print(f"{hex(byte)}", end=" ")
                print()
            s.close()
            del s
            print("Connection closed")
        except KeyboardInterrupt:
            break
        except TimeoutError:
            print("Timeout")
            s.close()
            del s
            continue
    s.close()
