#include <QString>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#ifndef CLIENT_H
#define CLIENT_H

class client
{
public:
    client(QString input);


    QString name;

    

    RSA * PublicKey;


    RSA * PrivateKey;


};

#endif // CLIENT_H
