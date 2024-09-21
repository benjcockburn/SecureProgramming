#include <fstream>
#include <iostream>

#include "JsonHandler.h"
#include "json.hpp"
#include "parseJsonFile.cpp"

/* can be compiled with "g++ -o test_validation test_validation.cpp
JsonHandler.cpp encrypt_AES.cpp generate_Keys.cpp
publicKeyStringConversion.cpp -lssl -lcrypto" */

int main() {
  JsonHandler handler;

  std::string file_name;
  std::cout << "Enter the name of the JSON file to test: ";
  std::cin >> file_name;

  nlohmann::json message = parseJsonFile(file_name);

  if (handler.validateMessage(message)) {
    std::cout << "Message is valid." << std::endl;
  } else {
    std::cout << "Message is invalid." << std::endl;
  }

  return 0;
}