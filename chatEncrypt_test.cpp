#include <iostream>
#include <vector>
#include <string>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include "json.hpp"  // Include a JSON library like nlohmann/json

using json = nlohmann::json;

// Function to Base64 encode data
std::string base64Encode(const std::vector<unsigned char>& data) {
    BIO* bio = BIO_new(BIO_f_base64());
    BIO* mem = BIO_new(BIO_s_mem());
    bio = BIO_push(bio, mem);
    
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // No newlines in output
    BIO_write(bio, data.data(), data.size());
    BIO_flush(bio);
    
    BUF_MEM* bufferPtr;
    BIO_get_mem_ptr(bio, &bufferPtr);
    
    std::string encoded(bufferPtr->data, bufferPtr->length);
    BIO_free_all(bio);
    return encoded;
}

// Function to perform AES encryption
bool aesEncrypt(const std::vector<unsigned char>& plaintext, std::vector<unsigned char>& ciphertext, std::vector<unsigned char>& key, std::vector<unsigned char>& iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int len;
    int ciphertext_len;
    
    // Generate a random AES key and IV
    key.resize(32);  // AES-256
    iv.resize(16);   // 128-bit IV
    RAND_bytes(key.data(), key.size());
    RAND_bytes(iv.data(), iv.size());
    
    if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data())) {
        std::cerr << "Error initializing AES encryption." << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    
    ciphertext.resize(plaintext.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));
    
    if (!EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext.data(), plaintext.size())) {
        std::cerr << "Error during AES encryption." << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    ciphertext_len = len;
    
    if (!EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len)) {
        std::cerr << "Error finalizing AES encryption." << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    ciphertext_len += len;
    
    ciphertext.resize(ciphertext_len);
    
    EVP_CIPHER_CTX_free(ctx);
    return true;
}

// Function to perform RSA encryption
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
    // Step 1: Create the inner "chat" JSON object
    json chatJson = {
        {"participants", {
            "<Base64 encoded list of fingerprints of participants, starting with sender>"
        }},
        {"message", "<Plaintext message>"}
    };

    // Convert the inner chat JSON to a string
    std::string chatString = chatJson.dump();

    // Convert chat string to a vector of unsigned chars
    std::vector<unsigned char> chatData(chatString.begin(), chatString.end());

    // Step 2: Encrypt the "chat" field with AES
    std::vector<unsigned char> aesKey, iv, aesEncryptedChat;
    if (!aesEncrypt(chatData, aesEncryptedChat, aesKey, iv)) {
        std::cerr << "AES encryption failed." << std::endl;
        return 1;
    }

    // Base64 encode the AES IV and AES-encrypted chat
    std::string ivBase64 = base64Encode(iv);
    std::string aesEncryptedChatBase64 = base64Encode(aesEncryptedChat);

    // Step 3: Load each recipient's public RSA key and encrypt the AES key for each
    std::vector<std::string> encryptedKeysBase64;
    std::vector<std::string> destinationServers = {
        "<Address of each recipient's destination server>"
    };

    for (const auto& server : destinationServers) {
        // Load the recipient's public key (for this example, let's use a single key file)
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

        // Encrypt the AES key with the recipient's RSA public key
        std::vector<unsigned char> encryptedKey;
        if (!rsaEncrypt(aesKey, encryptedKey, publicKey)) {
            std::cerr << "RSA encryption failed for recipient." << std::endl;
            RSA_free(publicKey);
            return 1;
        }

        // Base64 encode the encrypted AES key
        encryptedKeysBase64.push_back(base64Encode(encryptedKey));

        // Free the RSA public key after use
        RSA_free(publicKey);
    }

    // Step 4: Create the outer JSON structure
    json outerJson = {
        {"data", {
            {"type", "chat"},
            {"destination_servers", destinationServers},
            {"iv", ivBase64},
            {"symm_keys", encryptedKeysBase64},
            {"chat", aesEncryptedChatBase64}
        }}
    };

    // Convert the outer JSON to a string
    std::string outerJsonString = outerJson.dump();

    // Print the final encrypted JSON
    std::cout << "Encrypted JSON message: " << outerJsonString << std::endl;

    return 0;
}