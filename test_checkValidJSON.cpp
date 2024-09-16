#include <fstream>
#include <iostream>

#include "checkValidJSON.cpp"
#include "json.hpp"
#include "parseJsonFile.cpp"

// can be compliled with "g++ -o test_checkValidJSON test_checkValidJSON.cpp"

int main() {
  std::string file_name;
  std::cout << "Enter the name of the JSON file to test: ";
  std::cin >> file_name;

  nlohmann::json message = parseJsonFile(file_name);
  int last_counter = 12345;

  if (validate_message(message, last_counter)) {
    std::cout << "Message is valid." << std::endl;
  } else {
    std::cout << "Message is invalid." << std::endl;
  }

  return 0;
}