#include "client.h"

client::client(QString input) {
    this->name = input;

}

 std::string client::PublicKeyString(){
    return rsaPublicKeyToString(this->PublicKey);
 };