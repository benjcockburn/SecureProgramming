#ifndef GENERATE_KEYS_H
#define GENERATE_KEYS_H

#include <openssl/aes.h>
#include <openssl/rand.h>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string toHex(const unsigned char* data, int len);

bool generateAESKeyAndIV(std::vector<unsigned char>& aesKey,
                         std::vector<unsigned char>& iv);

#endif