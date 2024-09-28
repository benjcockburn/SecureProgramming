

#include "decrypt_RSA.h"

// RSA Decrypt function (for decrypting the AES key)
bool rsaDecrypt(const std::vector<unsigned char>& encryptedData, std::vector<unsigned char>& decryptedData, RSA* privateKey) {
    int rsaLen = RSA_size(privateKey);
    decryptedData.resize(rsaLen);

    int len = RSA_private_decrypt(encryptedData.size(), encryptedData.data(), decryptedData.data(), privateKey, RSA_PKCS1_OAEP_PADDING);
    if (len == -1) {
        std::cerr << "Error decrypting with RSA: " << ERR_get_error() << std::endl;
        return false;
    }

    decryptedData.resize(len);
    return true;
}




// Load RSA private key from a file (PEM format)
RSA* loadPrivateKeyFromFile(const std::string& privateKeyPath) {
    FILE* privKeyFile = fopen(privateKeyPath.c_str(), "r");
    if (!privKeyFile) {
        std::cerr << "Error opening private key file: " << privateKeyPath << std::endl;
        return nullptr;
    } else {
        std::cout << "Private Key Loaded" << std::endl;
    }
    RSA* rsa = PEM_read_RSAPrivateKey(privKeyFile, NULL, NULL, NULL);
    fclose(privKeyFile);
    return rsa;
}