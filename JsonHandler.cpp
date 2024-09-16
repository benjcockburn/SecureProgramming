#include "JsonHandler.h"

#include "ecodeBase_64.cpp"

// JSON Construction
nlohmann::json JsonHandler::constructSignedData(nlohmann::json data) {
  int currentCounter = ++counter;
  std::string dataStr = data.dump();
  std::string signature =
      base64Encode((dataStr + std::to_string(currentCounter)));

  return nlohmann::json{{"type", "signed_data"},
                        {"data", data},
                        {"counter", currentCounter},
                        {"signature", signature}};
}

nlohmann::json JsonHandler::constructHello(const std::string& publicKey) {
  nlohmann::json helloData = {{"type", "hello"}, {"public_key", publicKey}};

  return constructSignedData(helloData);
}

nlohmann::json JsonHandler::constructChat(
    const std::vector<std::string>& destinationServers,
    const std::vector<std::string>& encryptedKeys, const std::string& iv,
    const std::string& encryptedChatMessage) {
  std::vector<std::string> base64encodedKeys;

  for (const auto& key : encryptedKeys) {
    base64encodedKeys.push_back(base64Encode(key));
  }

  nlohmann::json chatData = {{"type", "chat"},
                             {"destination_servers", destinationServers},
                             {"iv", base64Encode(iv)},
                             {"symm_keys", base64encodedKeys},
                             {"chat", base64Encode(encryptedChatMessage)}};

  return constructSignedData(chatData);
}

nlohmann::json JsonHandler::constructPublicChat(
    const std::string& senderFingerprint, const std::string& message) {
  nlohmann::json publicChatData = {{"type", "public_chat"},
                                   {"sender", base64Encode(senderFingerprint)},
                                   {"message", message}};

  return constructSignedData(publicChatData);
}

nlohmann::json JsonHandler::constructClientListRequest() {
  return nlohmann::json{{"type", "client_list_request"}};
}

nlohmann::json JsonHandler::constructClientList(
    const std::vector<std::pair<std::string, std::vector<std::string>>>&
        serverClients) {
  nlohmann::json serversArray = nlohmann::json::array();

  for (const auto& server : serverClients) {
    nlohmann::json serverData = {{"address", server.first},
                                 {"clients", server.second}};

    serversArray.push_back(serverData);
  }

  nlohmann::json clientListResponse = {{"type", "client_list"},
                                       {"servers", serversArray}};

  return clientListResponse;
}

nlohmann::json JsonHandler::constructClientUpdate(
    const std::vector<std::string>& clients) {
  return nlohmann::json{{"type", "client_update"}, {"clients", clients}};
}

nlohmann::json JsonHandler::constructClientUpdateRequest() {
  return nlohmann::json{{"type", "client_update_request"}};
}

nlohmann::json JsonHandler::constructServerHello(const std::string& serverIP) {
  nlohmann::json serverHelloData = {{"type", "server_hello"},
                                    {"sender", serverIP}};

  return constructSignedData(serverHelloData);
}