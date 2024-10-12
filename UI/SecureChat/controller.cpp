#include "controller.h"

controller_sp::controller_sp(JsonHandler *jsonInput,int port)
{
    this->jsonHandler = jsonInput;
    this->port = port;

};

controller_sp::~controller_sp()
{
    this->thread_receiving_controller = false;
}

// handle counter updates and client updates in this from the python

bool controller_sp::start()
{
    // start sending server
    this->thread_receiving_controller = true;
    std::thread serverThread_controller(&controller_sp::server, this);

    // Detach the thread so it runs independently
    serverThread_controller.detach();
    // start receiving server
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    return this->result == -1;
}

int controller_sp::server()
{

    while (this->thread_receiving_controller)
    {

        const int PORT = port+1;
        const int BUFFER_SIZE = 1024;

        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1)
        {
            std::cerr << "Socket creation failed" << std::endl;
            this->result = 5;
            return 1;
        }

        sockaddr_in server_addr{};
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(PORT);

        if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        {
            std::cerr << "Bind failed" << std::endl;

            close(server_fd);
            this->result = 6;
            return 1;
        }

        if (listen(server_fd, 5) == -1)
        {
            std::cerr << "Listen failed" << std::endl;
            close(server_fd);
            this->result = 7;
            return 1;
        }

        std::cout << "Server listening on port " << PORT << " for operations..." << std::endl;

        while (true)
        {
            sockaddr_in client_addr{};
            socklen_t client_addr_len = sizeof(client_addr);
            int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
            if (client_fd == -1)
            {
                std::cerr << "Accept failed" << std::endl;
                continue;
            }

            std::cout << "Client connected" << std::endl;

            // Receive data in a loop
            char buffer[BUFFER_SIZE];
            ssize_t bytes_received;
            while ((bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0)) > 0)
            {
                buffer[bytes_received] = '\0'; // Null-terminate the buffer
                std::cout << "Received message: " << buffer << std::endl;

                // write to screen
                QString message = buffer;
                QString recipient = "Recipient";
                QString sender = "Sender";

                std::cout << message.toStdString() << std::endl;

                if (message.contains("counter_update"))
                {
                    std::cout<< "Counter update received"<<std::endl;
                    QStringList parts = message.split(" ");
                    if (parts.size() == 2)
                    {
                        bool ok;
                        int counterValue = parts[1].toInt(&ok); // Convert the second part to an integer

                        if (ok){
                            this->jsonHandler->updateCounter(counterValue);
                        }
                            


                    }
                }
            }

            if (bytes_received == -1)
            {
                std::cerr << "Receive failed" << std::endl;
            }
            else if (bytes_received == 0)
            {
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
