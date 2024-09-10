#ifndef LOG_H
#define LOG_H
#include <openssl/sha.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include "passwords.txt"

//dunno if object is the way forward but its what ive got here
class login{
    private:
        std::string user;
        //passwords to be encrypted
        std::string password;
        //check to see if user is real
        bool find_User(const std::string& filename, const std::string& user);
        //compares inputted user to confirm they exist
        std::string seach_User_Pass(std::string user);


    public:
        //only compare password is public, thats important im sure
        bool check_Password(std::string user,std::string pWord);




};


#endif