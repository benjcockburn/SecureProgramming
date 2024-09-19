#include <iostream>
#include <vector>
#include <string>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include "json.hpp"  // Include a JSON library like nlohmann/json

using json = nlohmann::json;

// Function to Base64 encode data
std::string base64Encode(const std::vector<unsigned char>& data) {
    BIO* bio = BIO_new(BIO_f_base64());
    BIO* mem = BIO_new(BIO_s_mem());
    bio = BIO_push(bio, mem);
    
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // Do not append newlines
    BIO_write(bio, data.data(), data.size());
    BIO_flush(bio);
    
    BUF_MEM* bufferPtr;
    BIO_get_mem_ptr(bio, &bufferPtr);
    
    std::string encoded(bufferPtr->data, bufferPtr->length);
    
    BIO_free_all(bio);
    return encoded;
}

bool rsaEncrypt(const std::vector<unsigned char>& data, std::vector<unsigned char>& encrypted, RSA* publicKey) {
    int rsaLen = RSA_size(publicKey);
    encrypted.resize(rsaLen);

    int len = RSA_public_encrypt(data.size(), data.data(), encrypted.data(), publicKey, RSA_PKCS1_OAEP_PADDING);
    if (len == -1) {
        std::cerr << "Error encrypting with RSA: " << ERR_get_error() << std::endl;
        return false;
    }

    encrypted.resize(len);
    return true;
}

int main() {
    // Create the JSON object
    json jsonObj = {
        {"data", {
            {"type", "public_chat"},
            {"sender", "<Base64 encoded fingerprint of sender>"},
            {"message", "<Plaintext message>"}
        }}
    };

    // Convert the JSON object to string
    std::string jsonString = jsonObj.dump();

    // Convert the JSON string to a vector of unsigned chars
    std::vector<unsigned char> data(jsonString.begin(), jsonString.end());

    // Load the recipient's public key from a PEM file
    FILE* pubKeyFile = fopen("recipient_public_key.pem", "r");
    if (!pubKeyFile) {
        std::cerr << "Error opening public key file." << std::endl;
        return 1;
    }
    RSA* publicKey = PEM_read_RSA_PUBKEY(pubKeyFile, nullptr, nullptr, nullptr);
    fclose(pubKeyFile);

    if (!publicKey) {
        std::cerr << "Error reading public key." << std::endl;
        return 1;
    }

    // Vector to store the encrypted data
    std::vector<unsigned char> encrypted;

    // Encrypt the data
    if (rsaEncrypt(data, encrypted, publicKey)) {
        // If encryption is successful, Base64 encode the result
        std::string encryptedBase64 = base64Encode(encrypted);

        // Print the encrypted, Base64 encoded result
        std::cout << "Encrypted message (Base64): " << encryptedBase64 << std::endl;
    } else {
        std::cerr << "Encryption failed." << std::endl;
    }

    // Free the RSA public key
    RSA_free(publicKey);

    return 0;
}