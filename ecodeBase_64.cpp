#include <iostream>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <string>

// Base64 Encoding Function
std::string base64Encode(const std::string& input) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;
    
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // Do not add newlines
    bio = BIO_push(b64, bio);

    BIO_write(bio, input.c_str(), input.length());
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    BIO_set_close(bio, BIO_NOCLOSE);
    std::string encodedData(bufferPtr->data, bufferPtr->length);
    BIO_free_all(bio);

    return encodedData;
}

// Base64 Decoding Function
std::string base64Decode(const std::string& encodedInput) {
    BIO *bio, *b64;
    char* buffer = (char*)malloc(encodedInput.length());
    
    // Setup the Base64 decode BIO chain
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_mem_buf(encodedInput.c_str(), encodedInput.length());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // Do not expect newlines in Base64 data
    bio = BIO_push(b64, bio);

    // Decode into buffer
    int decodedLength = BIO_read(bio, buffer, encodedInput.length());
    std::string decodedData(buffer, decodedLength);

    // Cleanup
    BIO_free_all(bio);
    free(buffer);

    return decodedData;
}