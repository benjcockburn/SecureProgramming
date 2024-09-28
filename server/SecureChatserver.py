import socket
import threading
import time

import json
import asyncio

import websockets


def clean_json_string(json_string):
    # Example cleanup logic (e.g., removing any trailing commas, fixing quotes, etc.)
    json_string = json_string.strip()
    # Add more cleanup logic if needed
    return json_string

# Define the host and port

# Create a socket object
SERVER_ADDRESS = '127.0.0.1'  # Localhost



counter_value = 0




class Server:
    def __init__(self, host, port):
        self.counter = counter_value
        self.host = host
        self.port = port
        self.neighbour_servers = []  # List of connected neighbour servers
        self.connected_clients = set()  # Track connected clients

    async def handler(self, websocket, path):
        self.connected_clients.add(websocket)
        addr = websocket.remote_address
        print(f"Accepted connection from {addr}")

        try:
            async for message in websocket:
                data = json.loads(message)
                print(f"Received message from {addr}: {data}")
                await self.process_message(data, websocket, addr)

        except (websockets.exceptions.ConnectionClosedError, json.JSONDecodeError) as e:
            print(f"Error processing message from {addr}: {e}")

        finally:
            self.connected_clients.remove(websocket)
            print(f"Connection to {addr} closed")

    async def process_message(self, message, websocket, addr):
        if message['type'] == 'client_update_request':
            print(f"Received client update request from {addr}")
            await self.send_client_update(websocket)

        elif message['type'] == 'server_hello':
            print(f"Received server hello from {addr}: {message['data']['sender']}")

        else:
            print(f"Unknown message type from {addr}: {message['type']}")

    async def send_client_update(self, websocket=None):
        client_update = {
            "type": "client_update",
            "clients": []  # Here, you can specify clients if needed
        }

        print(f"Sending client update: {client_update}")

        # Send to all neighbour servers
        for server in self.neighbour_servers:
            try:
                await server.send(json.dumps(client_update))
                print(f"Sent client update to neighbour server: {server.remote_address}")
            except Exception as e:
                print(f"Failed to send client update to {server.remote_address}: {e}")

        # Optionally send to the requesting server
        if websocket:
            await websocket.send(json.dumps(client_update))
            print(f"Sent client update to server {websocket.remote_address}")

    async def connect_to_neighbour(self, neighbour_ip, neighbour_port):
        try:
            uri = f"ws://{neighbour_ip}:{neighbour_port}"
            neighbour = await websockets.connect(uri)
            self.neighbour_servers.append(neighbour)

            server_hello = {
                "data": {
                    "type": "server_hello",
                    "sender": self.host
                }
            }
            await neighbour.send(json.dumps(server_hello))
            print(f"Connected to neighbour server at {neighbour_ip}:{neighbour_port}")

        except Exception as e:
            print(f"Failed to connect to neighbour server {neighbour_ip}:{neighbour_port}: {e}")

    async def relay_public(self, message):
        for websocket in self.neighbour_servers:
            try:
                await websocket.send(json.dumps(message))
                print(f"Sent public chat to neighbour server: {websocket.remote_address}")
            except Exception as e:
                print(f"Failed to send public chat to {websocket.remote_address}: {e}")

    def start(self):
        start_server = websockets.serve(self.handler, self.host, self.port)
        print(f"Server listening on {self.host}:{self.port}")

        asyncio.get_event_loop().run_until_complete(start_server)
        asyncio.get_event_loop().run_forever()




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
# Offset 1 || e.g. 8000 - Used to recieve client requests  // python server
#
# Offset 2 || e.g. 8001 - Used to send client updates (operations)  // cpp server
#
# Offset 3 || e.g. 8002 - Used to send chat from client to sever,  //python server
#
# Offset 4 || e.g. 42069 - Used to send chat from server to client. // cpp server

operations_type = {}
operations_type['status'] = 'wait'
threads_running = True

def start_server(port):
    global threads_running
    global counter_value
    global server
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

            if(message['data']['type']=='public_chat'):
                print("public chat!!")
                # server.relayPublic(message)


            # chat
            if(message['data']['type']=='chat'):
                print("chat!!")

            # 


        # Close the client socket after receiving the message
        client_socket.close()
        print("Connection closed. Waiting for a new message...\n")



def run(port):
# Server address and port
    global threads_running
    global counter_value
    while(threads_running):
        message="Hello!!! - from message port"
        time.sleep(9)

    # Create a socket object
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
            try:
                # Connect to the server
                client_socket.connect((SERVER_ADDRESS, port))
                
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
        data = client_socket.recv(4000)
        if not data:
            print("No data received, closing connection.")
        else:
            cleaned_string = clean_json_string(data.decode('utf-8'))
            # print(cleaned_string)
            message = json.loads(cleaned_string)
            print(f"Received: {message}")

            # hello

            # client list


        # Close the client socket after receiving the message
        client_socket.close()
        print("Connection closed. Waiting for a new message...\n")

def operations_send(port):
    global threads_running
    global counter_value
    global operations_type
    while(threads_running):
        
        if(operations_type['status']=='wait'):
            time.sleep(1)
            # print("waiting")
            message=""
            operation(input(),6)
            continue

        if(operations_type['status'] == 'counter_update'):
            message=f"counter_update {operations_type['value']}"
            operations_type['status']='wait'

    # Create a socket object
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
            try:
                # Connect to the server
                client_socket.connect((SERVER_ADDRESS, port))
                
                # Send the message
                client_socket.sendall(message.encode('utf-8'))
                
                print("Message sent successfully.")
            
            except ConnectionError as e:
                print(f"Connection error: {e}")

    # port should be 8001

    # send counter value back to client

    
def operation(type,value=0):
    global operations_type
    global counter_value
    if type == 'counter_update':
        operations_type['value']=value
        operations_type['status']='counter_update'





def OutsideServer_start():
    global server
    server.start()





if __name__ == "__main__":
    
    threads_running = True

    thread_send = threading.Thread(target=run,args=(42069,))
    thread_recieve = threading.Thread(target=start_server,args=(8002,))
    thread_operations_receive = threading.Thread(target=operations_receive,args=(8000,))
    thread_operations_send = threading.Thread(target=operations_send,args=(8001,))

    # thread_outside_server = threading.Thread(target=OutsideServer_start)

    thread_send.start()
    thread_recieve.start()
    thread_operations_receive.start()
    thread_operations_send.start()

    # thread_outside_server.start()
    server.start()

