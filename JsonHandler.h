#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <iostream>
#include <regex>
#include <string>

#include "json.hpp"

class JsonHandler {
 private:
  int counter = 0;
  nlohmann::json constructSignedData(nlohmann::json data);

  const std::regex base64_regex;
  bool is_base64(const std::string& str);

 public:
  JsonHandler();

  // JSON Construction
  nlohmann::json constructHello(const std::string& publicKey);
  nlohmann::json constructChat(
      const std::vector<std::string>& destinationServers,
      const std::vector<std::string>& encryptedKeys, const std::string& iv,
      const std::string& encryptedChatMessage);
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
};

#endif