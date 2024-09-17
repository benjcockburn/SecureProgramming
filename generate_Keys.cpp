#include <openssl/rand.h>
#include <openssl/aes.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>

// Helper function to convert binary to Base64 or hex (for debugging)
std::string toHex(const unsigned char* data, int len) {
    std::stringstream ss;
    ss << std::hex;
    for (int i = 0; i < len; i++)
        ss << std::setw(2) << std::setfill('0') << (int)data[i];
    return ss.str();
}

// Generate an AES key and IV
bool generateAESKeyAndIV(std::vector<unsigned char>& aesKey, std::vector<unsigned char>& iv, int keySize = 256) {
    aesKey.resize(keySize / 8); // AES key size: 32 bytes for 256-bit key
    iv.resize(AES_BLOCK_SIZE);  // IV size: 16 bytes (block size of AES)

    // Generate random AES key and IV
    if (!RAND_bytes(aesKey.data(), aesKey.size()) || !RAND_bytes(iv.data(), iv.size())) {
        std::cerr << "Error generating AES key or IV" << std::endl;
        return false;
    }

    return true;
}