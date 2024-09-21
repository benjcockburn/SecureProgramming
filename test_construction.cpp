#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>

#include <iostream>
#include <string>
#include <vector>

#include "JsonHandler.h"
#include "encrypt_AES.cpp"
#include "generate_Keys.cpp"
#include "json.hpp"

// can be compiled with "g++ -o test_construction test_construction.cpp
// JsonHandler.cpp -lssl -lcrypto"

int main() {
  JsonHandler handler;

  // Construct Hello
  std::string publicKey = "test_public_key";
  nlohmann::json helloJson = handler.constructHello(publicKey);
  std::cout << "Hello JSON: " << helloJson.dump(4) << std::endl << std::endl;

  // Construct Chat
  std::vector<std::string> destinationServers = {"server1", "server2"};
  std::vector<std::string> participants = {"fingerprint1", "fingerprint2"};
  std::string privateMessage = "Example Message";

  std::vector<RSA*> publicKeys;
  RSA* pubKey1 = loadPublicKey("public_key_example_1.pem");
  RSA* pubKey2 = loadPublicKey("public_key_example_2.pem");
  publicKeys.push_back(pubKey1);
  publicKeys.push_back(pubKey2);

  nlohmann::json chatJson = handler.constructChat(
      destinationServers, participants, privateMessage, publicKeys);

  std::cout << "Chat JSON: " << chatJson.dump(4) << std::endl << std::endl;

  // Consruct Public Chat
  std::string senderFingerprint = "senderFingerprint";
  std::string message = "Hello, world!";
  nlohmann::json publicChatJson =
      handler.constructPublicChat(senderFingerprint, message);
  std::cout << "Public Chat JSON: " << publicChatJson.dump(4) << std::endl
            << std::endl;

  // Construct Client List Request
  nlohmann::json clientListRequestJson = handler.constructClientListRequest();
  std::cout << "Client List Request JSON: " << clientListRequestJson.dump(4)
            << std::endl
            << std::endl;

  // Construct Client Update Request
  nlohmann::json clientUpdateRequestJson =
      handler.constructClientUpdateRequest();
  std::cout << "Client Update Request JSON: " << clientUpdateRequestJson.dump(4)
            << std::endl
            << std::endl;

  // Construct Server Hello
  std::string serverIP = "192.168.1.1";
  nlohmann::json serverHelloJson = handler.constructServerHello(serverIP);
  std::cout << "Server Hello JSON: " << serverHelloJson.dump(4) << std::endl
            << std::endl;

  return 0;
}