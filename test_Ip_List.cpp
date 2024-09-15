#include "ip_List.cpp"


int main() {
    std::string inputJson = R"({
        "type": "client_list",
        "servers": [
            {
                "address": "192.168.1.1",
                "clients": [
                    "RSA_PUBLIC_KEY_1",
                    "RSA_PUBLIC_KEY_2"
                ]
            },
            {
                "address": "192.168.1.2",
                "clients": [
                    "RSA_PUBLIC_KEY_3"
                ]
            }
        ]
    })";

    // Call the function and retrieve the map
    auto serverClientMap = parseClientList(inputJson);

    // Print the result
    for (const auto& pair : serverClientMap) {
        std::cout << "Server Address: " << pair.first << std::endl;
        std::cout << "Clients: ";
        for (const auto& client : pair.second) {
            std::cout << client << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}