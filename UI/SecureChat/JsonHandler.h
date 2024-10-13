

#ifndef JSONHANDLER_SP
#define JSONHANDLER_SP
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

#include "decrypt_AES.h"               // yes
#include "decrypt_RSA.h"               // yes
#include "ecodeBase_64.h"              // yes
#include "encrypt_AES.h"               // yes
#include "generate_Keys.h"             // yes
#include "publicKeyStringConversion.h" // yes
#include "json.hpp"

#include <openssl/rsa.h>

#include <iostream>
#include <regex>
#include <string>

class JsonHandler
{
private:
    int counter = 0;
    nlohmann::json constructSignedData(nlohmann::json data, std::string privateKey);

    const std::regex base64_regex;
    bool is_base64(const std::string &str);

    bool verifySignature(const nlohmann::json &message, std::string publicKey);

    std::string hash(const std::string& data);
    std::string signData(const std::string& data, std::string privateKey);

public:
    void updateCounter(int input);
    JsonHandler();

    // JSON Construction
    nlohmann::json constructHello(const std::string &publicKey, std::string privateKey);                   // sent to python
    nlohmann::json constructChat(                                                  // 
        const std::vector<std::string> &destinationServers,                        //
        const std::vector<std::string> &participants, const std::string message,   //
        const std::vector<std::string> &publicKeys, std::string privateKey);                               //
    nlohmann::json constructPublicChat(const std::string &senderFingerprint,       // sent to python 
                                       const std::string &message, std::string privateKey);                //
    nlohmann::json constructClientListRequest();                                   //
    nlohmann::json constructClientList(                                            //
        const std::vector<std::pair<std::string, std::vector<std::string>>> &      //
            serverClients);                                                        //
    nlohmann::json constructClientUpdate(const std::vector<std::string> &clients); //
    nlohmann::json constructClientUpdateRequest();                                 //
    nlohmann::json constructServerHello(const std::string &serverIP, std::string privateKey);              // in python

    // JSON Validation
    bool validateMessage(const nlohmann::json &message, std::string publicKey);

    // Determine Message Type
    std::string findMessageType(const nlohmann::json &message);

    // Decrypt Chat Message
    nlohmann::json decryptChat(const nlohmann::json& message,
                            RSA* rsa_privateKey);
};

#endif
