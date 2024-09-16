#include <iostream>
#include <regex>
#include <string>

#include "json.hpp"

/* This function is going to have its functionality put into JsonHandler */

const std::regex base64_regex(
    "^(?:[A-Za-z0-9+/]{4})*(?:[A-Za-z0-9+/]{2}==|[A-Za-z0-9+/]{3}=)?$");

bool is_base64(const std::string& str) {
  return std::regex_match(str, base64_regex);
}

bool validate_message(const nlohmann::json& message, int& last_counter) {
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

      if (!(message["counter"] > last_counter)) {
        std::cerr << "Counter value not greater than last counter."
                  << std::endl;
        return false;
      }

      // update the counter
      last_counter = message["counter"];

      // data
      if (!message["data"].contains("type")) {
        std::cerr << "Missing 'type' field in data section." << std::endl;
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

        if (!is_base64(message["data"]["public_key"])) {
          std::cerr << "Invalid base64 public_key." << std::endl;
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

      for (const auto& client : message["clients"]) {
        if (!is_base64(client)) {
          std::cerr << "Invalid base64 client." << std::endl;
          return false;
        }
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