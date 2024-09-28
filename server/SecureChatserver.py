import socket
import threading
import time

# Define the host and port

# Create a socket object
SERVER_ADDRESS = '127.0.0.1'  # Localhost
PORT = 42069

# Message to send


threads_running = True

def start_server():
    # Create a socket object
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Allow the server to reuse the same port immediately after closing
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    # Bind to the address and port
    server_socket.bind(('127.0.0.1', 65432))

    # Start listening for incoming connections
    server_socket.listen(1)
    print("Server is listening on port 65432...")

    while threads_running:
        # Accept the connection
        client_socket, client_address = server_socket.accept()
        print(f"Connection from {client_address}")

        # Receive data from the client
        data = client_socket.recv(1024)
        if not data:
            print("No data received, closing connection.")
        else:
            print(f"Received: {data.decode()}")

        # Close the client socket after receiving the message
        client_socket.close()
        print("Connection closed. Waiting for a new message...\n")



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

