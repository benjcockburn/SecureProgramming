#include <iostream>
#include <string>
#include <vector>

#include "JsonHandler.h"
#include "json.hpp"

// can be compiled with "g++ -o test_JsonHandler_combined
// test_JsonHandler_combined.cpp JsonHandler.cpp -lssl -lcrypto"

int main() {
  JsonHandler constructor;
  JsonHandler reciever;

  // Construct Hello
  std::string publicKey = "cHVibGljX2tleQ==";
  nlohmann::json helloJson = constructor.constructHello(publicKey);

  if (reciever.validateMessage(helloJson)) {
    std::cout << "Hello is valid." << std::endl;
  } else {
    std::cout << "Hello is invalid." << std::endl;
  }

  // Construct Chat (need to make a new test for the new version of the
  // function)

  /* std::vector<std::string> destinationServers = {"server1,", "server2"};
  std::vector<std::string> encryptedKeys = {"encryptedKey1", "encryptedKey2"};
  std::string iv = "initializationVector";
  std::string encryptedChatMessage = "encryptedMessage";
  nlohmann::json chatJson = constructor.constructChat(
      destinationServers, encryptedKeys, iv, encryptedChatMessage);

  if (reciever.validateMessage(chatJson)) {
    std::cout << "Chat is valid." << std::endl;
  } else {
    std::cout << "Chat is invalid." << std::endl;
  } */

  // Consruct Public Chat
  std::string senderFingerprint = "senderFingerprint";
  std::string message = "Hello, world!";
  nlohmann::json publicChatJson =
      constructor.constructPublicChat(senderFingerprint, message);

  if (reciever.validateMessage(publicChatJson)) {
    std::cout << "Public Chat is valid." << std::endl;
  } else {
    std::cout << "Public Chat is invalid." << std::endl;
  }

  // Construct Client List Request
  nlohmann::json clientListRequestJson =
      constructor.constructClientListRequest();

  if (reciever.validateMessage(clientListRequestJson)) {
    std::cout << "Client List Request is valid." << std::endl;
  } else {
    std::cout << "Client List Request is invalid." << std::endl;
  }

  // Construct Client Update Request
  nlohmann::json clientUpdateRequestJson =
      constructor.constructClientUpdateRequest();

  if (reciever.validateMessage(clientUpdateRequestJson)) {
    std::cout << "Client Update Request is valid." << std::endl;
  } else {
    std::cout << "Client Update Request is invalid." << std::endl;
  }

  // Construct Server Hello
  std::string serverIP = "192.168.1.1";
  nlohmann::json serverHelloJson = constructor.constructServerHello(serverIP);

  if (reciever.validateMessage(serverHelloJson)) {
    std::cout << "Server Hello is valid." << std::endl;
  } else {
    std::cout << "Server Hello is invalid." << std::endl;
  }

  std::cout << std::endl;

  // Replay attack
  if (reciever.validateMessage(serverHelloJson)) {
    std::cout << "Replay attack succeeded." << std::endl;
  } else {
    std::cout << "Replay attack prevented." << std::endl;
  }

  return 0;
}