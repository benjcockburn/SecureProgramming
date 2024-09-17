#include <openssl/evp.h>
#include <openssl/aes.h>
#include <vector>
#include <string>
#include <iostream>

// AES Decrypt function
bool aesDecrypt(const std::vector<unsigned char>& key, const std::vector<unsigned char>& iv,
                const std::vector<unsigned char>& ciphertext, std::string& plaintext) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int len = 0;
    int plaintextLen = 0;
    
    // Initialize the decryption operation
    if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.data(), iv.data())) {
        std::cerr << "Error initializing AES decryption" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    // Provide the ciphertext to be decrypted
    std::vector<unsigned char> buffer(ciphertext.size() + AES_BLOCK_SIZE);
    if (!EVP_DecryptUpdate(ctx, buffer.data(), &len, ciphertext.data(), ciphertext.size())) {
        std::cerr << "Error during AES decryption" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    plaintextLen = len;

    // Finalize the decryption
    if (!EVP_DecryptFinal_ex(ctx, buffer.data() + len, &len)) {
        std::cerr << "Error finalizing AES decryption" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    plaintextLen += len;

    // Convert decrypted data into string
    plaintext = std::string(buffer.begin(), buffer.begin() + plaintextLen);

    EVP_CIPHER_CTX_free(ctx);
    return true;
}