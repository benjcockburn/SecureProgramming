#include "login.h"
//sudo apt-get update
//sudo apt-get install libssl-dev
//needed for function to work just in case u dont have
std::string sha256(const std::string& str) {
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

bool login::find_User(const std::string& filename, const std::string& user){
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

std::string login::seach_User_Pass(std::string user){
    std::string password;
    //if user is in user storage return the stored encryped password
    return password;
}
bool login::check_Password(std::string user,std::string pWord){
    //first we check if the user is real
    if (find_User("passwords.txt" ,user) == 0){
        return 0;
    }


    //read the password from the file and compare to hashed input password
    std::string pass1 = seach_User_Pass(user);
    std::string pass2 = sha256(pWord);
    

    //then we compare and return true or false
    if (pass1 == pass2){
        return true;
    }
    else{
    return false;
    }




}