#include "messagehandler.h"




int connectAndSend(const char* server_ip, int port, const char* message) {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket!" << std::endl;
        return -1;
    }

    // Define the server address
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);  // Port number
    serverAddress.sin_addr.s_addr = inet_addr(server_ip);  // Server's IP

    // Connect to the server
    int connectionStatus = connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
    if (connectionStatus == -1) {
        std::cerr << "Error connecting to the server!" << std::endl;
        close(clientSocket);
        return -1;
    }

    // Send data to the server
    if (send(clientSocket, message, strlen(message), 0) < 0) {
        std::cerr << "Error sending data!" << std::endl;
        close(clientSocket);
        return -1;
    }
    std::cout << "Data sent: " << message << std::endl;

    // Close the socket
    close(clientSocket);
    return 0;  // Return 0 on success
}


messageHandler::messageHandler(QObject *parent)
    : QObject(parent) // Properly initializing QObject's parent
{
    // WebSocketClient ws_client;
    // std::string uri = "ws://localhost:8765";  // WebSocket server URL

    // ws_client.run(uri);
    this->start();
}



bool messageHandler::sendMessage(const char* message,int port){

    // construct the message in json

    // send to python to relay
        const char* server_ip = "127.0.0.1";



    // Call the abstracted function
    int result = connectAndSend(server_ip, port, message);
    if (result == 0) {
        std::cout << "Message sent successfully." << std::endl;
        return true;
    } else {
        std::cerr << "Failed to send message." << std::endl;
        return false;
    }



}

messageHandler:: ~messageHandler(){
    this->thread_receiving = false;
    this->thread_sending = false;
};


void messageHandler::start(){
    // start sending server
    this->thread_receiving=true;
    std::thread serverThread(&messageHandler::server, this);


    // Detach the thread so it runs independently
    serverThread.detach();
    // start receiving server

}

int messageHandler::server() {


    while(this->thread_receiving){


        const int PORT = 8003;
        const int BUFFER_SIZE = 1024;


            int server_fd = socket(AF_INET, SOCK_STREAM, 0);
            if (server_fd == -1) {
                std::cerr << "Socket creation failed" << std::endl;
                return 1;
            }

            sockaddr_in server_addr{};
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = INADDR_ANY;
            server_addr.sin_port = htons(PORT);

            if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
                std::cerr << "Bind failed" << std::endl;
                close(server_fd);
                return 1;
            }

            if (listen(server_fd, 5) == -1) {
                std::cerr << "Listen failed" << std::endl;
                close(server_fd);
                return 1;
            }

            std::cout << "Server listening on port " << PORT << " for messages..." << std::endl;

            while (true) {
                sockaddr_in client_addr{};
                socklen_t client_addr_len = sizeof(client_addr);
                int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
                if (client_fd == -1) {
                    std::cerr << "Accept failed" << std::endl;
                    continue;
                }

                std::cout << "Client connected" << std::endl;

                // Receive data in a loop
                char buffer[BUFFER_SIZE];
                ssize_t bytes_received;
                while ((bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0)) > 0) {
                    buffer[bytes_received] = '\0'; // Null-terminate the buffer
                    std::cout << "Received message: " << buffer << std::endl;

                    // write to screen
                    QString message = buffer;
                    QString recipient = "Recipient";
                    QString sender = "Sender";

                    // Emit the signal to send the message to the main thread
                    emit messageReceived(message, recipient, sender);


                }

                if (bytes_received == -1) {
                    std::cerr << "Receive failed" << std::endl;
                } else if (bytes_received == 0) {
                    std::cout << "Client disconnected" << std::endl;
                }

                // Close the client connection
                close(client_fd);
            }

            close(server_fd);
            return 0;
        }





    return 0;
}



