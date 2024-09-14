#include <iostream>
#include "login.cpp"

int main() {
    login loginObj;
    std::string username, password;

    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    if (loginObj.check_Password(username, password)) {
        std::cout << "Login successful." << std::endl;
    } else {
        std::cout << "Login failed." << std::endl;
    }

    return 0;
}