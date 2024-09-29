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
        self.connected_servers = set()  # Track connected servers
        self.client_list = []


    async def handler(self, websocket, path):
        self.connected_servers.add(websocket)
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
            self.connected_servers.remove(websocket)
            print(f"Connection to {addr} closed")

    async def process_message(self, message, websocket, addr):
        if message['data']['type'] == 'client_list_request':
            print(f"Received client update request from {addr}")
            await self.send_client_list(websocket)

        elif message['data']['type'] == 'server_hello':
            print(f"Received server hello from {addr}: {message['data']['sender']}")

        elif message['data']['type'] == 'connect':
            # print("about to attack")
            await self.connect_to_neighbour(message['data']['ip'],message['data']['port'])
            # print("did it work?")

        elif message['data']['type'] == 'chat':
            set_message(message)
        
        elif message['data']['type'] == 'public_chat':
            set_message(message)


        else:
            print(f"Unknown message type from {addr}: {message['type']}")

    async def send_client_list(self, websocket=None):
        client_update = {
            "type": "client",
            "servers": [
                {
                    "address": self.host,
                    "clients": [client for client in self.client_list]
                }
            ]
            # Here, you can specify clients if needed
        }

        print(f"Sending client list update: {client_update}")

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

    async def relay_private(self, message):
        print("relaying private message")
        print(message)
        destination = message['data']['destination_servers'][0]
        print(destination)

        # Split the string at the colon
        ip, port = destination.split(':')

        # Convert the port to an integer if needed
        port = int(port)
        try:
            uri = f"ws://{ip}:{port}" #todo: make this handle more then one server
            neighbour = await websockets.connect(uri)


            self.neighbour_servers.append(neighbour)

            await neighbour.send(json.dumps(message))
        except Exception as e:
                print(f"Failed to send private chat to {destination}: {e}")
        
        # print("back to people") Turn this on the route the message back to the user
        # set_message(message)
            
        

    def addClient(self,client):
        self.client_list.append(client)
        print(self.client_list)


    async def start(self):
        server = await websockets.serve(self.handler, self.host, self.port)
        print(f"Server listening on {self.host}:{self.port}")
        await server.wait_closed()



    # async def start(self):
    #     start_server = websockets.serve(self.handler, self.host, self.port)
    #     print(f"Server listening on {self.host}:{self.port}")

    #     # server.addClient("-----BEGIN PUBLIC KEY-----\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAs7PPRzrrm2ZknkfTYEA8\nhD7DKXQDUbMh2cQbdbPn0uwIHbbOWu9emEVAzovTCQQ33ocUuxwHo5EMFphkpjv4\nyRCPNCFHIG7LeXSPKdBwZTL6Vm8OXfRFzb5dpXQCbCwW2yXZsPvZrL/5ZWy1Be5R\nB9SH1OvIdopX1EhLZyGG21UCP51KAg9Y78CPdnHISb5Sruy8XujLs6zRbugXN/yL\nLrbzpeF9wUrhz4h204JfdoS1eJ01q9dvb2ybkf2tHu54yJD5slftt+sNPKx37zD5\nZ7Rh80KkKntJ2e9cgsZ16Gk+8SZJ50fgUjO2ce0tVCkNiuk+T1T7SDV137NbGCB1\nuwIDAQAB\n-----END PUBLIC KEY-----\n")
    #     # server.addClient("-----BEGIN PUBLIC KEY-----\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA0Htkylt7i1s2ZTkc0RVT\n25l2pR/HxsxBfAXRkUt9djgBMyy7VJo02v2LvirbKkc+5U7SoBpx0F37s2UF4tD5\nvHN8AsC2GHsIAKHpO87ZLi3mAFdoVu0zGhsk3VnEe+YrsdGPC9uuCTzl6JuKS3qB\nHAfhFiQiEZO0ykWeJhI1A97eHoA0Ed3GGUJArZ43hn9enOcU0lWhP/8NxeZSbZdD\n237L8cBijVgSzc23Bc6/ye7+sI+irg9s+TsmW3i/3hZnKrxeQCBQf1ZqLKoTllO8\nxfxzn+Pvk/mqx+vmBzD4mqMWayWtORwb9vNjXMJrd31yWNMd4JmabLtOaA6wK8nu\n6wIDAQAB\n-----END PUBLIC KEY-----\n")
    #     # loop = asyncio.get_event_loop()

    #     # Start both the WebSocket and socket servers concurrently
    #     await start_server


def set_message(chat):
    global reporting_message
    reporting_message = chat

#
# Ports n what they do
#
# 12345 - Outside connections coming in
server = Server('192.168.0.173', 12345)
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
# Offset 4 || e.g. 8003 - Used to send chat from server to client. // cpp server

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
        data = client_socket.recv(5000)
        if not data:
            print("No data received, closing connection.")
        else:
            message = json.loads(data.decode('utf-8'))
            print(f"Received: {message}")

            # handle each type

            # public chat

            if(message['data']['type']=='public_chat'):
                print("public chat!!")
                # server.relayPublic(message) TODO:


            # chat
            if(message['data']['type']=='chat'):
                print("chat!!")
                    #TODO:
            # 


        # Close the client socket after receiving the message
        client_socket.close()
        print("Connection closed. Waiting for a new message...\n")

reporting_message=""

def run(port):
    global server

# Server address and port
    global threads_running
    global counter_value
    global reporting_message
    while(threads_running):
        
        if(reporting_message==""):
            continue
        else:
            message=json.dumps(reporting_message)

    # Create a socket object
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
            try:
                # Connect to the server
                client_socket.connect((SERVER_ADDRESS, port))
                
                # Send the message
                client_socket.sendall(message.encode('utf-8'))
                
                print("Message sent successfully.")

                reporting_message=""
            
            except ConnectionError as e:
                print(f"Connection error: {e}")

# def operations_receive(port):
#     global threads_running
#     global counter_value
#     global server
#         # Create a socket object
#     server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

#     # Allow the server to reuse the same port immediately after closing
#     server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

#     # Bind to the address and port
#     server_socket.bind(('127.0.0.1', port))

#     # Start listening for incoming connections
#     server_socket.listen(1)
#     print(f"Server is listening on port {port} for operations...")

#     while threads_running:
#         # Accept the connection
#         client_socket, client_address = server_socket.accept()
#         print(f"Connection from {client_address}")

#         # Receive data from the client
#         data = client_socket.recv(4000)
#         if not data:
#             print("No data received, closing connection.")
#         else:
#             cleaned_string = clean_json_string(data.decode('utf-8'))
#             # print(cleaned_string)
#             message = json.loads(cleaned_string)
#             print(f"Received: {message}")

#             # hello
#             if(message['data']['type']=='hello'):
#                 print("client says hello")
#                 server.addClient(message['data']['public_key'])
            
#             if(message['data']['type']=='ping'):
#                 print("client pinging server")
                

#             # client list


#         # Close the client socket after receiving the message
#         client_socket.close()
#         print("Connection closed. Waiting for a new message...\n")
                


async def handle_client(client_socket, loop):
    print("handling")
    try:
        # Receive data in chunks
        while True:
            data = await loop.run_in_executor(None, client_socket.recv, 4000)

            print(data)
            
            if not data:
                print("No more data, closing connection.")
                break
            
            # Decode the received bytes into a JSON string
            json_data = data.decode("utf-8")
            
            try:
                # Parse the JSON
                parsed_data = json.loads(json_data)
                print("Received:", parsed_data)

                # Handle the parsed data
                if parsed_data.get('data', {}).get('type') == 'ping':
                    print("Client is pinging us")
                    response = json.dumps({"response": "pong"})
                    await loop.run_in_executor(None, client_socket.send, response.encode('utf-8'))

            except json.JSONDecodeError:
                print("Invalid JSON data received")
                break

    except Exception as e:
        print(f"Error: {e}")
    
    finally:
        client_socket.close()
        print("Connection closed")


async def start_socket_server(loop):
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind(("localhost", 8000))
    print("listening on 8000 for operations")
    server_socket.listen(5)
    
    while True:
        # Accept incoming client connection
        client_socket, _ = server_socket.accept()
        print("Client connected")

        

        data = client_socket.recv(4000)
        if not data:
            print("No data received, closing connection.")
        else:
            cleaned_string = clean_json_string(data.decode('utf-8'))
            # print(cleaned_string)
            message = json.loads(cleaned_string)
            print(f"Received: {message}")

            # hello
            if(message['data']['type']=='hello'):
                print("client says hello")
                server.addClient(message['data']['public_key'])
            
            if(message['data']['type']=='ping'):
                print("client pinging server")
            
            if(message['data']['type']=='connect'):
                await server.connect_to_neighbour(message['data']['ip'],message['data']['port'])



            if(message['data']['type']=='public_chat'):
                print("public chat!!")
                await server.relay_public(message) 


            # chat
            if(message['data']['type']=='chat'):
                print("chat!!")
                await server.relay_private(message) 



            
        client_socket.close()
        
        # Handle each client in a separate asyncio task
        # asyncio.create_task(handle_client(client_socket, loop))




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


loop = asyncio.get_event_loop()





async def main():
    loop = asyncio.get_event_loop()
    await asyncio.gather(
        server.start(),
        start_socket_server(loop)
    )
    loop.run_forever()





if __name__ == "__main__":

    
    
    threads_running = True

    thread_send = threading.Thread(target=run,args=(8003,))
    thread_recieve = threading.Thread(target=start_server,args=(8002,))
    # thread_operations_receive = threading.Thread(target=operations_receive,args=(8000,))  replaced with asyncio socket server
    thread_operations_send = threading.Thread(target=operations_send,args=(8001,))

    # thread_outside_server = threading.Thread(target=OutsideServer_start)

    thread_send.start()
    thread_recieve.start()
    # thread_operations_receive.start()
    thread_operations_send.start()

    asyncio.run(main())
    # server.start()

   