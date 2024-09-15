#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include "json.hpp"
#include <unordered_set>

// For convenience
using json = nlohmann::json;

// Function to merge new client keys into the existing client list, avoiding duplicates
void mergeClientKeys(std::vector<std::string>& existingClients, const std::vector<std::string>& newClients) {
    // Use an unordered set to avoid duplicate keys
    std::unordered_set<std::string> clientSet(existingClients.begin(), existingClients.end());
    
    // Add new clients if they don't already exist in the set
    for (const auto& client : newClients) {
        if (clientSet.find(client) == clientSet.end()) {
            existingClients.push_back(client);
            clientSet.insert(client);
        }
    }
}

// Function to parse JSON and store server addresses and client keys, avoiding duplicates
void parseClientList(const std::string& jsonString, std::unordered_map<std::string, std::vector<std::string>>& serverClientMap) {
    // Parse the JSON input
    json jsonData = json::parse(jsonString);

    // Check if type is "client_list"
    if (jsonData["type"] == "client_list") {
        // Iterate through the servers
        for (const auto& server : jsonData["servers"]) {
            std::string serverAddress = server["address"];
            std::vector<std::string> newClientKeys;

            // Collect the new clients
            for (const auto& client : server["clients"]) {
                newClientKeys.push_back(client);
            }

            // If the server already exists, merge the clients, otherwise add new entry
            if (serverClientMap.find(serverAddress) != serverClientMap.end()) {
                mergeClientKeys(serverClientMap[serverAddress], newClientKeys);
            } else {
                serverClientMap[serverAddress] = newClientKeys;
            }
        }
    }
}


