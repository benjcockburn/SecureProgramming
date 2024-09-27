import socket
import threading
import time

# Define the host and port

# Create a socket object
SERVER_ADDRESS = '127.0.0.1'  # Localhost
PORT = 42069

# Message to send


threads_running = True

def start_server(host='127.0.0.1', port=65432):
    global threads_running
    # Create a socket object
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        # Bind the socket to the host and port
        s.bind((host, port))
        # Listen for incoming connections (backlog = 1)
        s.listen(1)
        print(f"Server listening on {host}:{port}...")

        # Accept the incoming connection
        conn, addr = s.accept()
        with conn:
            print(f"Connected by {addr}")
            while True:
                # Receive data in chunks (1024 bytes in this case)
                data = conn.recv(1024)
                if not data:
                    # If no data is received, break the loop and close the connection
                    break
                print(f"Received: {data.decode('utf-8')}")
                # Optionally, send back a response
                conn.sendall(b'ACK: ' + data)


def run():
# Server address and port
    global threads_running
    while(threads_running):
        message=input()

    # Create a socket object
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
            try:
                # Connect to the server
                client_socket.connect((SERVER_ADDRESS, PORT))
                
                # Send the message
                client_socket.sendall(message.encode('utf-8'))
                
                print("Message sent successfully.")
            
            except ConnectionError as e:
                print(f"Connection error: {e}")



if __name__ == "__main__":
    
    threads_running = True

    thread_send = threading.Thread(target=run)
    thread_recieve = threading.Thread(target=start_server)


    thread_send.start()
    thread_recieve.start()

