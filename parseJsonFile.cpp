#include <iostream>
#include <fstream>
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

int main() {
    nlohmann::json data = parseJsonFile("test.json");
    std::cout << "Parsed JSON data: " << data.dump(4) << std::endl;
}