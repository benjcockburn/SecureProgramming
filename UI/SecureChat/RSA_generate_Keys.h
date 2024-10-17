//Bailey Atkinson (a1772413), Ben Cockburn (a1850196), Marcus Beckwith (a1852054)

#ifndef RSA_KEYGEN_H
#define RSA_KEYGEN_H

#include <openssl/rsa.h>

// Function to generate a 2048-bit private RSA key
RSA* generate_private_key();

// Function to extract the public key from the private key
RSA* generate_public_key(RSA* private_key);

#endif // RSA_KEYGEN_H
