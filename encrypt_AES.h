#ifndef ENCRYPT_AES_H
#define ENCRYPT_AES_H

#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

#include <cstring>
#include <iostream>
#include <vector>

bool aesEncrypt(const std::vector<unsigned char>& key,
                const std::vector<unsigned char>& iv,
                const std::string& plaintext,
                std::vector<unsigned char>& ciphertext);

bool rsaEncrypt(const std::vector<unsigned char>& data,
                std::vector<unsigned char>& encrypted, RSA* publicKey);

RSA* loadPublicKey(const std::string& publicKeyPath);

#endif