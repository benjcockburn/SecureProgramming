////Bailey Atkinson (a1772413), Ben Cockburn (a1850196), Marcus Beckwith (a1852054)

#ifndef PUBLICKEYSTRINGCONVERSION_H
#define PUBLICKEYSTRINGCONVERSION_H

#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

#include <sstream>

// public key conversion
std::string rsaPublicKeyToString(RSA* publicKey);
RSA* stringToRsaPublicKey(const std::string& publicKeyString);

// private key conversion
RSA* stringToRsaPrivateKey(const std::string& privateKeyString);

#endif
