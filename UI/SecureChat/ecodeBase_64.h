//Bailey Atkinson (a1772413), Ben Cockburn (a1850196), Marcus Beckwith (a1852054)
#ifndef base64_SP
#define base64_SP


#include <iostream>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <string>

std::string base64Encode(const std::string& input);

std::string base64Decode(const std::string& encodedInput);

#endif
