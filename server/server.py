import socket

# Define the host and port

# Create a socket object
SERVER_ADDRESS = '127.0.0.1'  # Localhost
PORT = 42069

# Message to send
message = 'Hello from Python!'


def run():
# Server address and port


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
    run()
