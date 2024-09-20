#include <iostream>
#include <fstream>
#include <vector>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include "encrypt_AES.cpp"
#include "generate_Keys.cpp"

// Functions being used
//bool aesEncrypt(const std::vector<unsigned char>& key, const std::vector<unsigned char>& iv, const std::string& plaintext, std::vector<unsigned char>& ciphertext);
//bool rsaEncrypt(const std::vector<unsigned char>& data, std::vector<unsigned char>& encrypted, RSA* publicKey);
//RSA* loadPublicKey(const std::string& publicKeyPath);
//bool generateAESKeyAndIV(std::vector<unsigned char>& aesKey, std::vector<unsigned char>& iv, int keySize = 256);

int main() {
    // 1. Load the recipient's public key from keyStore.pem
    std::string publicKeyPath = "keyStore.pem";
    RSA* publicKey = loadPublicKey(publicKeyPath);
    if (!publicKey) {
        std::cerr << "Failed to load public key from " << publicKeyPath << std::endl;
        return 1;
    }

    // 2. Generate an AES key and IV
    std::vector<unsigned char> aesKey;
    std::vector<unsigned char> iv;
    if (!generateAESKeyAndIV(aesKey, iv)) {
        std::cerr << "Error generating AES key and IV" << std::endl;
        RSA_free(publicKey); // Clean up
        return 1;
    }

    // 3. Encrypt the plaintext message using AES
    std::string plaintext = "This is a sample message to be encrypted.";
    std::vector<unsigned char> ciphertext;
    if (!aesEncrypt(aesKey, iv, plaintext, ciphertext)) {
        std::cerr << "Error encrypting message with AES" << std::endl;
        RSA_free(publicKey); // Clean up
        return 1;
    }

    // 4. Encrypt the AES key using RSA (with the recipient's public key)
    std::vector<unsigned char> encryptedAESKey;
    if (!rsaEncrypt(aesKey, encryptedAESKey, publicKey)) {
        std::cerr << "Error encrypting AES key with RSA" << std::endl;
        RSA_free(publicKey); // Clean up
        return 1;
    }

    // Output the results
    std::cout << "AES-encrypted message (in hex): ";
    for (unsigned char byte : ciphertext) {
        std::cout << std::hex << static_cast<int>(byte);
    }
    std::cout << std::endl;

    std::cout << "RSA-encrypted AES key (in hex): ";
    for (unsigned char byte : encryptedAESKey) {
        std::cout << std::hex << static_cast<int>(byte);
    }
    std::cout << std::endl;

    // 5. Clean up
    RSA_free(publicKey);
    return 0;
}