#include <iostream>
#include <string>
#include <vector>

#include "JsonHandler.h"
#include "json.hpp"

/* can be compiled with "g++ -o test_JsonHandler_combined
test_JsonHandler_combined.cpp JsonHandler.cpp encrypt_AES.cpp generate_Keys.cpp
publicKeyStringConversion.cpp -lssl -lcrypto" */

int main() {
  JsonHandler constructor;
  JsonHandler reciever;

  // public keys
  std::vector<std::string> publicKeys;
  std::string publicKey1 =
      "-----BEGIN PUBLIC KEY-----\n"
      "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAs7PPRzrrm2ZknkfTYEA8hD7DKXQD"
      "UbMh2cQbdbPn0uwIHbbOWu9emEVAzovTCQQ33ocUuxwHo5EMFphkpjv4yRCPNCFHIG7LeXSP"
      "KdBwZTL6Vm8OXfRFzb5dpXQCbCwW2yXZsPvZrL/"
      "5ZWy1Be5RB9SH1OvIdopX1EhLZyGG21UCP51KAg9Y78CPdnHISb5Sruy8XujLs6zRbugXN/"
      "yLLrbzpeF9wUrhz4h204JfdoS1eJ01q9dvb2ybkf2tHu54yJD5slftt+"
      "sNPKx37zD5Z7Rh80KkKntJ2e9cgsZ16Gk+8SZJ50fgUjO2ce0tVCkNiuk+"
      "T1T7SDV137NbGCB1uwIDAQAB\n"
      "-----END PUBLIC KEY-----\n";
  std::string publicKey2 =
      "-----BEGIN PUBLIC KEY-----\n"
      "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA0Htkylt7i1s2ZTkc0RVT25l2pR/"
      "HxsxBfAXRkUt9djgBMyy7VJo02v2LvirbKkc+"
      "5U7SoBpx0F37s2UF4tD5vHN8AsC2GHsIAKHpO87ZLi3mAFdoVu0zGhsk3VnEe+"
      "YrsdGPC9uuCTzl6JuKS3qBHAfhFiQiEZO0ykWeJhI1A97eHoA0Ed3GGUJArZ43hn9enOcU0l"
      "WhP/8NxeZSbZdD237L8cBijVgSzc23Bc6/ye7+sI+irg9s+TsmW3i/"
      "3hZnKrxeQCBQf1ZqLKoTllO8xfxzn+Pvk/"
      "mqx+vmBzD4mqMWayWtORwb9vNjXMJrd31yWNMd4JmabLtOaA6wK8nu6wIDAQAB\n"
      "-----END PUBLIC KEY-----\n";
  publicKeys.push_back(publicKey1);
  publicKeys.push_back(publicKey2);

  // Construct Hello
  nlohmann::json helloJson = constructor.constructHello(publicKey1);

  if (reciever.validateMessage(helloJson)) {
    std::cout << "Hello is valid." << std::endl;
  } else {
    std::cout << "Hello is invalid." << std::endl;
  }

  // Construct Chat
  std::vector<std::string> destinationServers = {"server1", "server2"};
  std::vector<std::string> participants = {"fingerprint1", "fingerprint2"};
  std::string privateMessage = "Example Message";

  nlohmann::json chatJson = constructor.constructChat(
      destinationServers, participants, privateMessage, publicKeys);

  if (reciever.validateMessage(chatJson)) {
    std::cout << "Chat is valid." << std::endl;
  } else {
    std::cout << "Chat is invalid." << std::endl;
  }

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

  // Construct Client List
  std::vector<std::pair<std::string, std::vector<std::string>>> serverClients;

  std::string server1_address = "server1.example.com";
  std::vector<std::string> server1_clients = publicKeys;
  std::pair<std::string, std::vector<std::string>> server1 = {server1_address,
                                                              server1_clients};
  serverClients.push_back(server1);

  std::string server2_address = "server2.example.com";
  std::vector<std::string> server2_clients = {publicKey1};
  std::pair<std::string, std::vector<std::string>> server2 = {server2_address,
                                                              server2_clients};
  serverClients.push_back(server2);

  nlohmann::json clientListJson =
      constructor.constructClientList(serverClients);

  if (reciever.validateMessage(clientListJson)) {
    std::cout << "Client List is valid." << std::endl;
  } else {
    std::cout << "Client List is invalid." << std::endl;
  }

  // Construct Client Update
  std::vector<std::string> clients = publicKeys;

  nlohmann::json clientUpdateJson = constructor.constructClientUpdate(clients);

  if (reciever.validateMessage(clientUpdateJson)) {
    std::cout << "Client Update is valid." << std::endl;
  } else {
    std::cout << "Client Update is invalid." << std::endl;
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