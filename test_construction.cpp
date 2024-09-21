#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>

#include <iostream>
#include <string>
#include <vector>

#include "JsonHandler.h"
#include "encrypt_AES.h"
#include "generate_Keys.h"
#include "json.hpp"

// can be compiled with "g++ -o test_construction test_construction.cpp
// JsonHandler.cpp encrypt_AES.cpp generate_Keys.cpp -lssl -lcrypto"

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