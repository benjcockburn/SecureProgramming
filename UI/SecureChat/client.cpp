//Bailey Atkinson (a1772413), Ben Cockburn (a1850196), Marcus Beckwith (a1852054)
#include "client.h"

std::string createFingerPrint_client(const std::string &pemPublicKey)
{
    // Load RSA public key
    BIO *bio = BIO_new_mem_buf(pemPublicKey.c_str(), -1);
    RSA *rsa = PEM_read_bio_RSA_PUBKEY(bio, nullptr, nullptr, nullptr);
    BIO_free(bio);

    if (!rsa)
    {
        std::cerr << "Failed to load RSA public key." << std::endl;
        return "";
    }

    // Compute SHA-256 hash
    unsigned char hash[SHA256_DIGEST_LENGTH];
    if (SHA256((unsigned char *)pemPublicKey.c_str(), pemPublicKey.length(), hash) == nullptr)
    {
        std::cerr << "Failed to compute SHA-256." << std::endl;
        RSA_free(rsa);
        return "";
    }

    // Convert hash to std::string for Base64 encoding
    std::string hashStr(reinterpret_cast<char *>(hash), SHA256_DIGEST_LENGTH);

    // Base64 encode the SHA-256 hash
    std::string base64EncodedHash = base64Encode(hashStr);
    RSA_free(rsa);

    return base64EncodedHash;
}

client::client(QString input)
{
    this->name = input;
}

void client::setPublicKey(RSA *input)
{

    this->PublicKey = input;

    this->fingerprint= createFingerPrint_client(this->PublicKeyString());
}

void client::setPrivateKey(RSA *input){
this->PrivateKey=input;
};



std::string client::PublicKeyString()
{
    return rsaPublicKeyToString(this->PublicKey);
};
