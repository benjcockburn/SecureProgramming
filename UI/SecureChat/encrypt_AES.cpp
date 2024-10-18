//Bailey Atkinson (a1772413), Ben Cockburn (a1850196), Marcus Beckwith (a1852054)

#include "encrypt_AES.h"

// AES Encrypt function
bool aesEncrypt(const std::vector<unsigned char>& key, const std::vector<unsigned char>& iv,
                const std::string& plaintext, std::vector<unsigned char>& ciphertext) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int len = 0;
    int ciphertextLen = 0;
    
    // Initialize the encryption operation
    if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.data(), iv.data())) {
        std::cerr << "Error initializing AES encryption" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    // Provide the message to be encrypted
    ciphertext.resize(plaintext.size() + AES_BLOCK_SIZE); // Make room for padding
    if (!EVP_EncryptUpdate(ctx, ciphertext.data(), &len, (unsigned char*)plaintext.data(), plaintext.size())) {
        std::cerr << "Error during AES encryption" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    ciphertextLen = len;

    // Finalize the encryption
    if (!EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len)) {
        std::cerr << "Error finalizing AES encryption" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    ciphertextLen += len;
    ciphertext.resize(ciphertextLen); // Adjust the size to the final length

    EVP_CIPHER_CTX_free(ctx);
    return true;
}

// RSA Encrypt function
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

RSA* loadPublicKey(const std::string& publicKeyPath) {
    FILE* pubKeyFile = fopen(publicKeyPath.c_str(), "r");
    if (!pubKeyFile) {
        std::cerr << "Error opening public key file" << std::endl;
        return nullptr;
    }else {
        std::cout << "Public Key Loaded" << std::endl;
    }
    RSA* rsa = PEM_read_RSA_PUBKEY(pubKeyFile, NULL, NULL, NULL);
    fclose(pubKeyFile);
    return rsa;
}
