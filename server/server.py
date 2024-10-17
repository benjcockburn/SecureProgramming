import asyncio
import websockets
import socket
import json
counter = 0

neighbour_servers = []  # List of connected neighbour servers
connected_servers = set()  # Track connected servers
client_list = []

async def relay_public(message):
    print(f"Forwarded to: {neighbour_servers}")
    
    tasks = []
    for server in neighbour_servers:
        ip, port = server.split(':')
        port = int(port)  # Convert the port to an integer if needed
        # Create an async task for each send operation and store it in the tasks list
        tasks.append(asyncio.create_task(send_public(ip, port, message)))

    # Optionally, you can await the completion of all tasks (if you want to handle them)
    await asyncio.gather(*tasks)

async def send_public(ip, port, message):
    try:
        print(f"sending message to {ip}:{port}")
        uri = f"ws://{ip}:{port}"
        neighbour = await websockets.connect(uri)
        await neighbour.send(json.dumps(message))
    except Exception as e:
        print(f"Failed to send private chat to {ip}:{port}: {e}")


async def relay_private(message):
    global neighbour_servers
    list_of_recievers = message['data']['destination_servers']
    
    for each in list_of_recievers:
        print("..\n\neach\n\n"+each)
        ipea, portea = each.split(':')
        if each not in neighbour_servers:
            print(f"adding: {each}")
            await addneighbour(each)
        
        await send_public(ipea,portea,message)
        
        # send_pbulic()

async def addneighbour(destination):
    global neighbour_servers
    print(f"desst: {destination}")
    neighbour_servers.append(destination)
    print(f"All servers: {neighbour_servers}")

async def echo(websocket, path):
    async for message in websocket:
        # await websocket.send(f"Echo: {message}")
        print(f"message: {message}")
        message= json.loads(message)
        ## incoming connections from outside
        if message['data']['type']=='chat':
            message_display(json.dumps(message))

        elif message['data']['type']=='public_chat':
            message_display(json.dumps(message))
        
async def addClient(client):
    global client_list
    client_list.append(client)
    print(client_list)

async def start_websocket_server():
    async with websockets.serve(echo, "0.0.0.0", 12345):
        print("WebSocket server running on ws://0.0.0.0:12345")
        await asyncio.Future()  # Run forever

async def handle_client(reader, writer):
    while True:
        data = await reader.readline()
        if not data:
            break
        message = data.decode().strip()
        message= json.loads(message)
        print(f"Received from local socket: {message}")
        if(message['data']['type']=='hello'):
            print("client says hello")
            await addClient(message['data']['public_key'])
            # relay to known servers
            
        elif(message['data']['type']=='awwake'or message['data']['type']=='ping'):
            print("client pinging server")
            
        if(message['data']['type']=='connect'):
            manual = message['data']['ip']+":"+message['data']['port']

            
            if manual not in neighbour_servers:
                print(f"adding: {manual}")
                await addneighbour(manual)
            # await addneighbour(message['data']['ip']+":"+message['data']['port'])



        elif(message['data']['type']=="public_chat"):
            print("public chat!!")
            await relay_public(message) 

        


        # chat
        elif(message['data']['type']=='chat'):
            print("chat!!")
            await relay_private(message) 

        writer.write(f"Echo: {message}\n".encode())
        await writer.drain()

async def operations_socket():
    server = await asyncio.start_server(handle_client, '127.0.0.1', 8000)
    print("Local socket server running on tcp://127.0.0.1:8000")
    async with server:
        await server.serve_forever()

async def main():
    # Run both servers concurrently
    await asyncio.gather(
        start_websocket_server(),
        operations_socket(),
        
    )




def message_display(message,port=8003):
    print("tying to sending message mesasge")
    # message=json.dumps(reporting_message)

    # Create a socket object
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
        try:
            # Connect to the server
            client_socket.connect(("127.0.0.1", port))
            
                # Send the message
            client_socket.sendall(message.encode('utf-8'))
                
            print("Message sent successfully.")
            
            
        except ConnectionError as e:
            print(f"Connection error: {e}")

def operation_message(message,port=8001):
    print("tying to sending ops mesasge")
    # message=json.dumps(reporting_message)

    # Create a socket object
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
        try:
            # Connect to the server
            client_socket.connect(("127.0.0.1", port))
            
                # Send the message
            client_socket.sendall(message.encode('utf-8'))
                
            print("Message sent successfully.")
            reporting_message=""
            
        except ConnectionError as e:
            print(f"Connection error: {e}")



threads_running=True

if __name__ == "__main__":

    

    asyncio.run(main())
