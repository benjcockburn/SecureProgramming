import socket
import threading
import time

# Define the host and port

# Create a socket object
SERVER_ADDRESS = '127.0.0.1'  # Localhost



counter_value = 0

import socket
import json

class Server:
    def __init__(self, host, port):
        self.counter = counter_value
        self.host = host
        self.port = port
        self.neighbour_servers = []  # List of connected neighbour servers

    def start(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.bind((self.host, self.port))
            s.listen()
            print(f"Server listening on {self.host}:{self.port}")

            while True:
                conn, addr = s.accept()
                print(f"Accepted connection from {addr}")
                with conn:
                    self.handle_connection(conn, addr)

    def handle_connection(self, conn, addr):
        print(f"Server connected to {addr}")
        try:
            while True:
                data = conn.recv(1024)
                if not data:
                    print(f"Connection closed by {addr}")
                    break

                message = json.loads(data.decode('utf-8'))
                print(f"Received message from {addr}: {message}")
                self.process_message(message, conn, addr)

        except (ConnectionResetError, json.JSONDecodeError) as e:
            print(f"Error processing message from {addr}: {e}")
        finally:
            conn.close()
            print(f"Connection to {addr} closed")

    def process_message(self, message, conn, addr):
        if message['type'] == 'client_update_request':
            print(f"Received client update request from {addr}")
            self.send_client_update(conn)
        
        elif message['type'] == 'server_hello':
            print(f"Received server hello from {addr}: {message['data']['sender']}")
        
        else:
            print(f"Unknown message type from {addr}: {message['type']}")

    def send_client_update(self, conn=None):
        client_update = {
            "type": "client_update",
            "clients": []  # Here, you can specify clients if needed
        }

        print(f"Sending client update: {client_update}")

        # Send to all neighbour servers
        for server in self.neighbour_servers:
            try:
                server.send(json.dumps(client_update).encode('utf-8'))
                print(f"Sent client update to neighbour server: {server.getpeername()}")
            except Exception as e:
                print(f"Failed to send client update to {server.getpeername()}: {e}")

        # Optionally send to the requesting server
        if conn:
            conn.send(json.dumps(client_update).encode('utf-8'))
            print(f"Sent client update to server {conn.getpeername()}")

    def connect_to_neighbour(self, neighbour_ip, neighbour_port):
        try:
            neighbour = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            neighbour.connect((neighbour_ip, neighbour_port))
            self.neighbour_servers.append(neighbour)

            server_hello = {
                "data": {
                    "type": "server_hello",
                    "sender": self.host
                }
            }
            neighbour.send(json.dumps(server_hello).encode('utf-8'))
            print(f"Connected to neighbour server at {neighbour_ip}:{neighbour_port}")

        except Exception as e:
            print(f"Failed to connect to neighbour server {neighbour_ip}:{neighbour_port}: {e}")



#
# Ports n what they do
#
# 12345 - Outside connections coming in
server = Server('localhost', 12345)
#
# TODO: make this check if already client is on by renedevous with 7999 from cpp
#
# 7999 - Intial connection checker, client asks for avaiable ports and server responds #TODO: Implement this
#
# Offset 1 || e.g. 8000 - Used to recieve client requests
#
# Offset 2 || e.g. 8001 - Used to send client updates (operations)
#
# Offset 3 || e.g. 8002 - Used to send chat from client to sever, 



threads_running = True

def start_server(port):
    global threads_running
    global counter_value
    # Create a socket object
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Allow the server to reuse the same port immediately after closing
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    # Bind to the address and port
    server_socket.bind(('127.0.0.1', port))

    # Start listening for incoming connections
    server_socket.listen(1)
    print(f"Server is listening on port {port} for messages from client...")

    while threads_running:
        # Accept the connection
        client_socket, client_address = server_socket.accept()
        print(f"Connection from {client_address}")

        # Receive data from the client
        data = client_socket.recv(1024)
        if not data:
            print("No data received, closing connection.")
        else:
            message = json.loads(data.decode('utf-8'))
            print(f"Received: {message}")

            # handle each type

            # public chat

            # chat

            # 


        # Close the client socket after receiving the message
        client_socket.close()
        print("Connection closed. Waiting for a new message...\n")



def run():
# Server address and port
    global threads_running
    global counter_value
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

def operations_receive(port):
    global threads_running
    global counter_value
        # Create a socket object
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Allow the server to reuse the same port immediately after closing
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    # Bind to the address and port
    server_socket.bind(('127.0.0.1', port))

    # Start listening for incoming connections
    server_socket.listen(1)
    print(f"Server is listening on port {port} for operations...")

    while threads_running:
        # Accept the connection
        client_socket, client_address = server_socket.accept()
        print(f"Connection from {client_address}")

        # Receive data from the client
        data = client_socket.recv(1024)
        if not data:
            print("No data received, closing connection.")
        else:
            message = json.loads(data.decode('utf-8'))
            print(f"Received: {message}")

            # hello

            # client list


        # Close the client socket after receiving the message
        client_socket.close()
        print("Connection closed. Waiting for a new message...\n")

def operations_send(port):
    global threads_running
    global counter_value

    # port should be 8001

    # send counter value back to client

    





def OutsideServer_start():
    global server
    server.start()





if __name__ == "__main__":
    
    threads_running = True

    thread_send = threading.Thread(target=run)
    thread_recieve = threading.Thread(target=start_server,args=(8002,))
    thread_operations_receive = threading.Thread(target=operations_receive,args=(8000,))
    thread_operations_send = threading.Thread(target=operations_send,args=(8001,))

    thread_outside_server = threading.Thread(target=OutsideServer_start)

    thread_send.start()
    thread_recieve.start()
    thread_operations_receive.start()
    thread_operations_send.start()

    thread_outside_server.start()
