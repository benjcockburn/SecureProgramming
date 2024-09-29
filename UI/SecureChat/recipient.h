#ifndef RECIPIENT_H
#define RECIPIENT_H

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

class recipient
{
public:

    recipient();
    recipient(std::string server,RSA * PublicRSA);

    std::string dest_server;
    std::string fingerPrint;
    

        void setPublicKey(RSA *input);

    std::string PublicKeyString();

        RSA *PublicKey;


};

#endif // RECIPIENT_H
