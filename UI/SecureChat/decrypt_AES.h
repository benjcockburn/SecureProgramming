//Bailey Atkinson (a1772413), Ben Cockburn (a1850196), Marcus Beckwith (a1852054)

#ifndef aesDecrypt_SP
#define aesDecrypt_SP
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <vector>
#include <string>
#include <iostream>

bool aesDecrypt(const std::vector<unsigned char>& key, const std::vector<unsigned char>& iv,
                const std::vector<unsigned char>& ciphertext, std::string& plaintext);

#endif