#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <openssl/sha.h>

//sudo apt-get update
//sudo apt-get install libssl-dev
//needed for function to work just in case u dont have

class login {
public:
    static std::string sha256(const std::string& str);
    bool find_User(const std::string& filename, const std::string& user);
    std::string search_User_Pass(const std::string& filename, const std::string& searchString);
    bool check_Password(const std::string& user, const std::string& pWord);
};

// SHA-256 hash function
std::string login::sha256(const std::string& str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

// Function to find a user in the file
bool login::find_User(const std::string& filename, const std::string& user) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string firstColumn;
        iss >> firstColumn; // Read the first column (word before any space or tab)

        if (firstColumn == user) {
            return true; // Found an exact match
        }
    }

    file.close();
    return false; // No match found
}

// Function to search for the user's password in the file
std::string login::search_User_Pass(const std::string& filename, const std::string& searchString) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        return ""; // Return an empty string if the file can't be opened
    }

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string firstColumn, nextColumn;

        iss >> firstColumn; // Read the first column (word before any space or tab)

        if (firstColumn == searchString && iss >> nextColumn) { // If the first column matches, and there's a next word
            return nextColumn; // Return the next column (second word)
        }
    }

    infile.close();
    return ""; // Return an empty string if no match was found
}

// Function to check if the user's password is correct
bool login::check_Password(const std::string& user, const std::string& pWord) {
    // First, we check if the user exists
    if (!find_User("passwords.txt", user)) {
        return false;
    }

    // Read the password from the file and compare it to the hashed input password
    std::string storedPasswordHash = search_User_Pass("passwords.txt", user);
    std::string inputPasswordHash = sha256(pWord);

    // Then we compare and return true or false
    return (storedPasswordHash == inputPasswordHash);
}

