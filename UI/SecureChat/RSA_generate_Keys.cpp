//Bailey Atkinson (a1772413), Ben Cockburn (a1850196), Marcus Beckwith (a1852054)
#include "generate_Keys.h"
#include <openssl/pem.h>
#include <openssl/bn.h>
#include <iostream>

RSA* generate_private_key() {
    const int key_length = 2048;  // Fixed key length of 2048 bits
    unsigned long exponent = RSA_F4;
    RSA* rsa = RSA_new();
    BIGNUM* bn = BN_new();
    BN_set_word(bn, exponent);

    // Generate the RSA key pair
    if (RSA_generate_key_ex(rsa, key_length, bn, nullptr) != 1) {
        std::cerr << "Error generating RSA key pair." << std::endl;
        RSA_free(rsa);
        BN_free(bn);
        return nullptr;
    }

    // Cleanup
    BN_free(bn);
    return rsa;  // The private key is part of this structure
}

RSA* generate_public_key(RSA* private_key) {
    if (!private_key) {
        std::cerr << "Invalid private key." << std::endl;
        return nullptr;
    }

    // Extract the public key from the private key
    RSA* public_key = RSAPublicKey_dup(private_key);
    if (!public_key) {
        std::cerr << "Error extracting public key." << std::endl;
    }

    return public_key;
}
