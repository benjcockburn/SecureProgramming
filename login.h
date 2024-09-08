#ifndef LOG_H
#define LOG_H
#include <string>


class login{
    private:
        std::string user;
        //passwords to be encrypted
        std::string password;
        //compares inputted user to confirm they exist
        std::string seach_User(std::string user);

    public:
        bool check_Password(std::string user,std::string pWord);




};


#endif