#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include "json.hpp"

// For convenience
using json = nlohmann::json;


// Function to parse JSON and store server addresses and client keys
std::unordered_map<std::string, std::vector<std::string>> parseClientList(const std::string& jsonString) {
    std::unordered_map<std::string, std::vector<std::string>> serverClientMap;
    
    // Parse the JSON input
    json jsonData = json::parse(jsonString);

    // Check if type is "client_list"
    if (jsonData["type"] == "client_list") {
        // Iterate through the servers
        for (const auto& server : jsonData["servers"]) {
            std::string serverAddress = server["address"];
            std::vector<std::string> clientKeys;

            // Iterate through the clients
            for (const auto& client : server["clients"]) {
                clientKeys.push_back(client);
            }

            // Store server address and client keys in the map
            serverClientMap[serverAddress] = clientKeys;
        }
    }

    return serverClientMap;
}


