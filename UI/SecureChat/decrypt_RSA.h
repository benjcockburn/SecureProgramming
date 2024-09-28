
#ifndef DECRYPT_RSA_SP
#define DECRYPT_RSA_SP

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <vector>
#include <string>
#include <iostream>

bool rsaDecrypt(const std::vector<unsigned char>& encryptedData, std::vector<unsigned char>& decryptedData, RSA* privateKey);

RSA* loadPrivateKeyFromFile(const std::string& privateKeyPath);

#endif