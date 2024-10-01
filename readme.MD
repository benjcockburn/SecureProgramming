hello

sudo apt install libssl-dev

run ./run.sh


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
2. The **Local Server** forwards messages to an **Outside Server** for secure processing.
3. The **Outside Server** relays messages to the **Receiving Local Server**.
4. The **Receiving Local Server** delivers messages to the destination **Client**.

## Prerequisites

### Client-Side (Qt Application)
To run the **Network Chat** client application, you must have **Qt** and its dependencies installed.

#### Installation for Qt:
1. Install the Qt framework from the [official Qt website](https://www.qt.io/download).
2. Make sure to include the required modules such as `QtCore`, `QtWidgets`, and any other necessary Qt components for the project.

### Server-Side (Python Application)
To run the server application, ensure you have the necessary Python dependencies installed.

#### Installation for Server:
1. Install Python (3.x) from [python.org](https://www.python.org/downloads/).
2. Install the required dependencies by running the following command:

   ```bash
   pip install -r requirements.txt
   ```

   Ensure that `requirements.txt` includes all necessary libraries such as `socket`, `cryptography`, and others required for server operations.

## Usage

### Running the Client

1. Navigate to the **client directory**.
2. Run the client using Qt by executing the following commands:

   ```bash
   ./NetworkChatClient
   ```

### Running the Server

1. Navigate to the **server directory**.
2. Start the server by running:

   ```bash
   python server.py
   ```

3. The server will listen for incoming connections and route messages securely between clients.

## Contribution Guidelines

1. Fork the repository.
2. Make your changes in a new branch.
3. Create a pull request for review.

## License

This project is licensed under the MIT License.

---

Let me know if you'd like any changes or additional details!