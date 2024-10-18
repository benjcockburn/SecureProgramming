## AUTHORS

Bailey Atkinson (a1772413)
Ben Cockburn (a1850196)
Marcus Beckwith (a1852054)

# Network Chat

**Network Chat** is a secure messaging application designed for secure communication between clients. It operates using a **client-server-outside-server-client** model, whilst using the OLAF/Neighbourhood protocol v1.1.3 to ensure proper communication between other implementations.

### Warning

This project has vulnerabilities that lead to instablity in the code, please try and break it :)

## Project Overview

This project is part of the **Secure Programming** course and aims to demonstrate secure programming practices in network-based communications. The software includes a **client-side application** built using **Qt** written in cpp and a **server-side application** that uses local sockets and websockets that is written in **Python**.

## Features

- **Secure Communication**: Messages are securely transmitted between clients using encryption.
- **Client-Server Architecture**: The system operates with multiple servers acting as intermediaries for secure message routing.
- **Multithreading**: The client supports multithreaded operations for efficient data handling.

## Architecture

The application follows a **client-server-outside-server-client** model:
1. The **Client** sends messages to the **Local Server**.
2. The **Local Server** forwards secure messages to an **Outside Server** for relay.
3. The **Outside Server** relays messages to the **Receiving Local Server**.
4. The **Receiving Local Server** delivers messages to the destination **Outside Server**.
5. The **Outside Server** delivers messages to the destination **Local Server**.
5. The **Local Server** delivers messages to the destination **Client**.

Kinda complex, but the **Local Server** and the **Outside Server** are the same python, its passing socket data through to websockets to be processed.

## Prerequisites

### Operating System
#### Recommended:
- Linux (ubuntu-20.04)
- MacOS

#### Will partially work:
- WSL (ports on WSL cannot connect outwards to other servers)

### Client-Side (Qt Application)
To run the **Network Chat** client application, you must have **Qt** and its dependencies installed.

#### Installation for Qt & Python3:

```bash
sudo apt update
```
```bash
sudo apt install python3-pip libssl-dev cmake qt5-qmake qtbase5-dev
```
```bash
pip install websockets asyncio
```

## Building

### Building the client for **MAC**

1. Download QT Creator from the internet {https://www.qt.io/download-qt-installer}
2. Open the ```CMakeLists.txt``` as a project file
3. Build for release
4. Hit the play button in the bottom left

### Building the Client

1. Navigate to the **client directory** (```<Submission>/UI/SecureChat/```)
2. Run the client using Qt by executing the following commands:

```bash
cmake .
```
```bash
make
```

## Usage

### Running the Server

1. Navigate to the **server directory** (```<Submission>/server/```)
2. Start the server by running:

```bash
python3 server.py
```

3. The server will listen for incoming connections and route messages securely between servers as according to protocol.
4. Nothing more needs to be done

### Using the client

First run the client

```bash
./SecureChat
```

Now you'll be presented a screen with these options:

### Login Page

- Nickname
   - Just words
- Input Box (for your nickname)
   - Input a nickname if you desire
   - has no affect on anything except the visual outgoing messages
- Retry Connection - Push Button
   - If the python fails to connect, the client is useless, therefore if it fails to connect you'll be unable to advance
- Enter the chat room - Push Button
   - Allows the user to enter the chat room and prints their public key to the console, for sharing between clients

### Chat Page

- Logged on as: ``<Nickname>``
- Should have a public key displayed
- Combobox with other users and public chat option
- Chat message box
- send message button
- File path button
- Upload file button
- ip input box for external servers
- port input box for external servers
- tryConnecting, sends the IP and Port to the server and defines it as a part of the neigbour and will send messages to that server

Tutorial

0. If the button to "LOAD Keys" is greyed out, start the python server ``python3 server.py``
1. Enter a nickname
2. Press Enter Chat ROom
3. Add the IPV4 of the server and its port you want to send to in the two input boxes
   - the default port for our implementation is 12345
4. Send a public chat to test
5. Input a new user's public key and their ip address with port
   - address formatted as such ```192.168.0.1:25565```
   - Public Key as Such in **ONE LINE** ```-----BEGIN PUBLIC KEY-----\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAs7PPRzrrm2ZknkfTYEA8\nhD7DKXQDUbMh2cQbdbPn0uwIHbbOWu9emEVAzovTCQQ33ocUuxwHo5EMFphkpjv4\nyRCPNCFHIG7LeXSPKdBwZTL6Vm8OXfRFzb5dpXQCbCwW2yXZsPvZrL/5ZWy1Be5R\nB9SH1OvIdopX1EhLZyGG21UCP51KAg9Y78CPdnHISb5Sruy8XujLs6zRbugXN/yL\nLrbzpeF9wUrhz4h204JfdoS1eJ01q9dvb2ybkf2tHu54yJD5slftt+sNPKx37zD5\nZ7Rh80KkKntJ2e9cgsZ16Gk+8SZJ50fgUjO2ce0tVCkNiuk+T1T7SDV137NbGCB1\nuwIDAQAB\n-----END PUBLIC KEY-----\n``` **ONE LINE**
6. Press "add user"
7. They will appear as an option in the comboBox with their fingerprint, so select the newest one
8. Send another message, this one will be encrypted!
