#ifndef LOG_H
#define LOG_H
#include <string>
#include <iostream>
#include <cstring>

//dunno if object is the way forward but its what ive got here
class login{
    private:
        std::string user;
        //passwords to be encrypted
        std::string password;
        //compares inputted user to confirm they exist
        std::string seach_User(std::string user);

    public:
        //only compare password is public, thats important im sure
        bool check_Password(std::string user,std::string pWord);




};


#endif