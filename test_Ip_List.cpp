#include "ip_List.cpp"


int main() {
    // Example JSON inputs
    std::string inputJson1 = R"({
        "type": "client_list",
        "servers": [
            {
                "address": "192.168.1.1",
                "clients": [
                    "RSA_PUBLIC_KEY_1",
                    "RSA_PUBLIC_KEY_2"
                ]
            }
        ]
    })";

    std::string inputJson2 = R"({
        "type": "client_list",
        "servers": [
            {
                "address": "192.168.1.1",
                "clients": [
                    "RSA_PUBLIC_KEY_3"
                ]
            },
            {
                "address": "192.168.1.2",
                "clients": [
                    "RSA_PUBLIC_KEY_4"
                ]
            }
        ]
    })";

    // Map to store server addresses and client keys
    std::unordered_map<std::string, std::vector<std::string>> serverClientMap;

    // Parse first JSON
    parseClientList(inputJson1, serverClientMap);

    // Parse second JSON
    parseClientList(inputJson2, serverClientMap);

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