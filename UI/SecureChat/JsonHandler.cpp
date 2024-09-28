#include "JsonHandler.h"



JsonHandler::JsonHandler()
    : base64_regex(
          "^(?:[A-Za-z0-9+/]{4})*(?:[A-Za-z0-9+/]{2}==|[A-Za-z0-9+/]{3}=)?$") {}

/* JSON Construction */

// Construct Signed Data
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

// Construct Hello
nlohmann::json JsonHandler::constructHello(const std::string& publicKey) {
  nlohmann::json helloData = {{"type", "hello"}, {"public_key", publicKey}};

  return constructSignedData(helloData);
}

// Construct Chat
nlohmann::json JsonHandler::constructChat(
    const std::vector<std::string>& destinationServers,
    const std::vector<std::string>& participants, const std::string message,
    const std::vector<std::string>& publicKeys) {
  std::vector<std::string> base64participants;
  for (const auto& participant : participants) {
    base64participants.push_back(base64Encode(participant));
  }
  nlohmann::json chatBlock = {{"participants", base64participants},
                              {"message", message}};
  std::string chatDump = chatBlock.dump();

  std::vector<unsigned char> aesKey, iv;
  generateAESKeyAndIV(aesKey, iv);

  std::vector<unsigned char> encrypted_chatDump;
  aesEncrypt(aesKey, iv, chatDump, encrypted_chatDump);
  std::string str_encrypted_chatDump(encrypted_chatDump.begin(),
                                     encrypted_chatDump.end());

  std::vector<std::string> base64encodedKeys;

  for (const auto& publicKey : publicKeys) {
    RSA* rsa_publicKey = stringToRsaPublicKey(publicKey);
    std::vector<unsigned char> encryptedKey;
    rsaEncrypt(aesKey, encryptedKey, rsa_publicKey);
    std::string str_encryptedKey(encryptedKey.begin(), encryptedKey.end());
    base64encodedKeys.push_back(base64Encode(str_encryptedKey));
  }

  std::string str_iv(iv.begin(), iv.end());

  nlohmann::json chatData = {{"type", "chat"},
                             {"destination_servers", destinationServers},
                             {"iv", base64Encode(str_iv)},
                             {"symm_keys", base64encodedKeys},
                             {"chat", base64Encode(str_encrypted_chatDump)}};

  return constructSignedData(chatData);
}

// Construct Public Chat
nlohmann::json JsonHandler::constructPublicChat(
    const std::string& senderFingerprint, const std::string& message) {
  nlohmann::json publicChatData = {{"type", "public_chat"},
                                   {"sender", base64Encode(senderFingerprint)},
                                   {"message", message}};

  return constructSignedData(publicChatData);
}

// Construct Client List Request
nlohmann::json JsonHandler::constructClientListRequest() {
  return nlohmann::json{{"type", "client_list_request"}};
}

// Construct Client List
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

// Construct Client Update
nlohmann::json JsonHandler::constructClientUpdate(
    const std::vector<std::string>& clients) {
  return nlohmann::json{{"type", "client_update"}, {"clients", clients}};
}

// Construct Client Update Request
nlohmann::json JsonHandler::constructClientUpdateRequest() {
  return nlohmann::json{{"type", "client_update_request"}};
}

// Construct Server Hello
nlohmann::json JsonHandler::constructServerHello(const std::string& serverIP) {
  nlohmann::json serverHelloData = {{"type", "server_hello"},
                                    {"sender", serverIP}};

  return constructSignedData(serverHelloData);
}

/* JSON Validation */
bool JsonHandler::is_base64(const std::string& str) {
  return std::regex_match(str, base64_regex);
}

bool JsonHandler::verifySignature(const nlohmann::json& message) {
  std::string type = message["type"];
  if (type != "signed_data") {
    std::cerr << "Wrong message type, expected 'signed_data'" << std::endl;
    return false;
  }

  std::string dataStr = message["data"].dump();
  int message_counter = message["counter"];

  std::string signature = message["signature"];

  std::string recreated_signature =
      base64Encode((dataStr + std::to_string(message_counter)));

  return recreated_signature == signature;
}

bool JsonHandler::validateMessage(const nlohmann::json& message) {
  try {
    if (!message.contains("type")) {
      std::cerr << "Missing 'Type' field." << std::endl;
      return false;
    }

    std::string message_type = message["type"];

    // signed_data
    if (message_type == "signed_data") {
      if (!message.contains("data") || !message.contains("counter") ||
          !message.contains("signature")) {
        std::cerr << "Invalid signed_data message. Missing required fields."
                  << std::endl;
        return false;
      }

      if (!is_base64(message["signature"])) {
        std::cerr << "Invalid base64 signature." << std::endl;
        return false;
      }

      if (!(message["counter"] > counter)) {
        std::cerr << "Counter value not greater than last counter."
                  << std::endl;
        return false;
      }

      // update the counter
      counter = message["counter"];

      // data
      if (!message["data"].contains("type")) {
        std::cerr << "Missing 'type' field in data section." << std::endl;
        return false;
      }

      // verify signature
      if (!(verifySignature(message))) {
        std::cerr << "Signature does not match data section." << std::endl;
        return false;
      }

      std::string data_type = message["data"]["type"];

      // hello message
      if (data_type == "hello") {
        if (!message["data"].contains("public_key")) {
          std::cerr << "Invalid hello message. Missing public_key."
                    << std::endl;
          return false;
        }
      }

      // chat message
      else if (data_type == "chat") {
        if (!message["data"].contains("destination_servers") ||
            !message["data"].contains("iv") ||
            !message["data"].contains("symm_keys") ||
            !message["data"].contains("chat")) {
          std::cerr << "Invalid chat message. Missing required fields."
                    << std::endl;
          return false;
        }

        if (!is_base64(message["data"]["iv"])) {
          std::cerr << "Invalid base64 iv." << std::endl;
          return false;
        }

        for (const auto& key : message["data"]["symm_keys"]) {
          if (!is_base64(key)) {
            std::cerr << "Invalid base64 symm_key." << std::endl;
            return false;
          }
        }

        if (!is_base64(message["data"]["chat"])) {
          std::cerr << "Invalid base64 chat." << std::endl;
          return false;
        }
      }

      // public_chat message
      else if (data_type == "public_chat") {
        if (!message["data"].contains("sender") ||
            !message["data"].contains("message")) {
          std::cerr << "Invalid public_chat message. Missing required fields."
                    << std::endl;
          return false;
        }

        if (!is_base64(message["data"]["sender"])) {
          std::cerr << "Invalid base64 sender." << std::endl;
          return false;
        }
      }

      else if (data_type == "server_hello") {
        if (!message["data"].contains("sender")) {
          std::cerr << "Invalid server_hello message. Missing sender."
                    << std::endl;
          return false;
        }
      }

      // if it does not match the above categories
      else {
        std::cerr << "Unknown data type." << std::endl;
        return false;
      }
    }

    // client_list_request
    else if (message_type == "client_list_request") {
    }

    // client_list
    else if (message_type == "client_list") {
      if (!message.contains("servers")) {
        std::cerr << "Invalid client_list message. Missing servers."
                  << std::endl;
        return false;
      }
    }

    // client_update
    else if (message_type == "client_update") {
      if (!message.contains("clients")) {
        std::cerr << "Invalid client_update message. Missing clients."
                  << std::endl;
        return false;
      }
    }

    // client_update_request
    else if (message_type == "client_update_request") {
    }

    // if it does not match the above categories
    else {
      std::cerr << "Unknown message type." << std::endl;
      return false;
    }

    return true;
  } catch (const nlohmann::json::exception& e) {
    std::cerr << "JSON parsing error: " << e.what() << std::endl;
    return false;
  }
}

/* Determine Message Type */
std::string JsonHandler::findMessageType(const nlohmann::json& message) {
  std::string message_type = message["type"];

  if (message_type == "signed_data") {
    return message["data"]["type"];
  }

  return message_type;
}

/* Decrypt Chat Message */
nlohmann::json JsonHandler::decryptChat(const nlohmann::json& message,
                                        const std::string& privateKey) {
  std::string base64_iv = message["data"]["iv"];
  std::vector<std::string> base64encodedKeys = message["data"]["symm_keys"];
  std::string base64_encryptedChatDump = message["data"]["chat"];

  std::string str_iv = base64Decode(base64_iv);
  std::string str_encrypted_chatDump = base64Decode(base64_encryptedChatDump);

  RSA* rsa_privateKey = stringToRsaPrivateKey(privateKey);

  std::vector<unsigned char> aesKey;

  for (const auto& base64encodedKey : base64encodedKeys) {
    std::string str_encryptedKey = base64Decode(base64encodedKey);
    std::vector<unsigned char> encryptedKey(str_encryptedKey.begin(),
                                            str_encryptedKey.end());

    std::vector<unsigned char> temp_aesKey;
    if (rsaDecrypt(encryptedKey, temp_aesKey, rsa_privateKey)) {
      aesKey = temp_aesKey;
      break;
    }
  }

  if (aesKey.empty()) {
    throw std::runtime_error("AES key decryption failed");
  }

  std::vector<unsigned char> iv(str_iv.begin(), str_iv.end());
  std::vector<unsigned char> encrypted_chatDump(str_encrypted_chatDump.begin(),
                                                str_encrypted_chatDump.end());

  std::string decrypted_chatDump;
  aesDecrypt(aesKey, iv, encrypted_chatDump, decrypted_chatDump);

  nlohmann::json chatBlock = nlohmann::json::parse(decrypted_chatDump);

  std::vector<std::string> base64participants = chatBlock["participants"];
  std::vector<std::string> participants;
  for (const auto& base64participant : base64participants) {
    participants.push_back(base64Decode(base64participant));
  }

  chatBlock["participants"] = participants;
  return chatBlock;
}