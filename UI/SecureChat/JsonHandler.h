

#ifndef JSONHANDLER_SP
#define JSONHANDLER_SP
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>

#include "decrypt_AES.h" // yes
#include "decrypt_RSA.h" // yes
#include "ecodeBase_64.h" // yes
#include "encrypt_AES.h" // yes
#include "generate_Keys.h"// yes
#include "publicKeyStringConversion.h" // yes
#include "json.hpp"

#include <openssl/rsa.h>

#include <iostream>
#include <regex>
#include <string>



class JsonHandler {
 private:
  int counter = 0;
  nlohmann::json constructSignedData(nlohmann::json data);

  const std::regex base64_regex;
  bool is_base64(const std::string& str);

  bool verifySignature(const nlohmann::json& message);

 public:
 void updateCounter(int input);
  JsonHandler();

  // JSON Construction
  nlohmann::json constructHello(const std::string& publicKey);
  nlohmann::json constructChat(
      const std::vector<std::string>& destinationServers,
      const std::vector<std::string>& participants, const std::string message,
      const std::vector<std::string>& publicKeys);
  nlohmann::json constructPublicChat(const std::string& senderFingerprint,
                                     const std::string& message);
  nlohmann::json constructClientListRequest();
  nlohmann::json constructClientList(
      const std::vector<std::pair<std::string, std::vector<std::string>>>&
          serverClients);
  nlohmann::json constructClientUpdate(const std::vector<std::string>& clients);
  nlohmann::json constructClientUpdateRequest();
  nlohmann::json constructServerHello(const std::string& serverIP);

  // JSON Validation
  bool validateMessage(const nlohmann::json& message);

  // Determine Message Type
  std::string findMessageType(const nlohmann::json& message);

  // Decrypt Chat Message
  nlohmann::json decryptChat(const nlohmann::json& message,
                             const std::string& privateKey);
};

#endif