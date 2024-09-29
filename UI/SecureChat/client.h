#include <QString>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#include "ecodeBase_64.h"
#include <iostream>
#include <string>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

#include "publicKeyStringConversion.h"

#ifndef CLIENT_H
#define CLIENT_H

class client
{
public:
    client(QString input);

    QString name;

    void setPublicKey(RSA *input);
void setPrivateKey(RSA *input);

    std::string PublicKeyString();

    std::string fingerprint;
        


    RSA *PrivateKey;

    RSA *PublicKey;


};

#endif // CLIENT_H
