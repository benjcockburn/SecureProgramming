//Bailey Atkinson (a1772413), Ben Cockburn (a1850196), Marcus Beckwith (a1852054)
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
