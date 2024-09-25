#include "messagehandler.h"

messageHandler::messageHandler() {

    // WebSocketClient ws_client;
    // std::string uri = "ws://localhost:8765";  // WebSocket server URL

    // ws_client.run(uri);
    this->start();
}



bool messageHandler::sendMessage(){

    return false;

}

messageHandler:: ~messageHandler(){
    this->thread_receiving = false;
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


        const int PORT = 42069;
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

            std::cout << "Server listening on port " << PORT << "..." << std::endl;

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




