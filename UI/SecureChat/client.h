#include <QString>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include "publicKeyStringConversion.h"

#ifndef CLIENT_H
#define CLIENT_H

class client
{
public:
    client(QString input);


    QString name;

    

    RSA * PublicKey;
    std::string PublicKeyString();


    RSA * PrivateKey;


};

#endif // CLIENT_H
