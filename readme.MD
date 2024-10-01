# Network Chat

**Network Chat** is a secure messaging application designed for secure communication between clients. It operates using a **client-server-outside-server-client** model, whilst using the OLAF/Neighbourhood protocol v1.1.3 to ensure proper communication between other implementations.

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

#### Will half work:
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

## Usage

### Running the Client

1. Navigate to the **client directory** (```<Submission>/UI/SecureChat/```)
2. Run the client using Qt by executing the following commands:

```bash
cmake .
```
```bash
make
```
```bash
./SecureChat
```

### Running the Server

1. Navigate to the **server directory** (```<Submission>/server/```)
2. Start the server by running:

```bash
python3 server.py
```

3. The server will listen for incoming connections and route messages securely between servers as according to protocol.