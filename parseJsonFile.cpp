#include <fstream>
#include <iostream>
#include <string>

#include "json.hpp"

nlohmann::json parseJsonFile(const std::string& filename) {
  std::ifstream file(filename);

  if (!file.is_open()) {
    throw std::runtime_error("Unable to open " + filename);
  }

  nlohmann::json jsonData;
  file >> jsonData;

  return jsonData;
}