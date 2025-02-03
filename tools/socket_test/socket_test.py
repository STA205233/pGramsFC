import socket

def main():
    # Create a socket object
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Get local machine name
    host = ""

    port = 0

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