#include <iostream>
#include <string>
#include <vector>

#include "JsonHandler.h"
#include "json.hpp"

/* can be compiled with "g++ -o test_decryption test_decryption.cpp
JsonHandler.cpp encrypt_AES.cpp generate_Keys.cpp publicKeyStringConversion.cpp
-lssl -lcrypto" */

int main() {
  JsonHandler constructor;
  JsonHandler reciever;

  std::vector<std::string> destinationServers = {"server1", "server2"};
  std::vector<std::string> participants = {"fingerprint1", "fingerprint2"};
  std::string privateMessage = "Example Message";

  std::vector<std::string> publicKeys;
  std::string publicKey1 =
      "-----BEGIN PUBLIC KEY-----\n"
      "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAs7PPRzrrm2ZknkfTYEA8hD7DKXQD"
      "UbMh2cQbdbPn0uwIHbbOWu9emEVAzovTCQQ33ocUuxwHo5EMFphkpjv4yRCPNCFHIG7LeXSP"
      "KdBwZTL6Vm8OXfRFzb5dpXQCbCwW2yXZsPvZrL/"
      "5ZWy1Be5RB9SH1OvIdopX1EhLZyGG21UCP51KAg9Y78CPdnHISb5Sruy8XujLs6zRbugXN/"
      "yLLrbzpeF9wUrhz4h204JfdoS1eJ01q9dvb2ybkf2tHu54yJD5slftt+"
      "sNPKx37zD5Z7Rh80KkKntJ2e9cgsZ16Gk+8SZJ50fgUjO2ce0tVCkNiuk+"
      "T1T7SDV137NbGCB1uwIDAQAB\n"
      "-----END PUBLIC KEY-----\n";
  std::string publicKey2 =
      "-----BEGIN PUBLIC KEY-----\n"
      "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA0Htkylt7i1s2ZTkc0RVT25l2pR/"
      "HxsxBfAXRkUt9djgBMyy7VJo02v2LvirbKkc+"
      "5U7SoBpx0F37s2UF4tD5vHN8AsC2GHsIAKHpO87ZLi3mAFdoVu0zGhsk3VnEe+"
      "YrsdGPC9uuCTzl6JuKS3qBHAfhFiQiEZO0ykWeJhI1A97eHoA0Ed3GGUJArZ43hn9enOcU0l"
      "WhP/8NxeZSbZdD237L8cBijVgSzc23Bc6/ye7+sI+irg9s+TsmW3i/"
      "3hZnKrxeQCBQf1ZqLKoTllO8xfxzn+Pvk/"
      "mqx+vmBzD4mqMWayWtORwb9vNjXMJrd31yWNMd4JmabLtOaA6wK8nu6wIDAQAB\n"
      "-----END PUBLIC KEY-----\n";
  publicKeys.push_back(publicKey1);
  publicKeys.push_back(publicKey2);

  std::string privateKey1 =
      "-----BEGIN PRIVATE KEY-----\n"
      "MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQCzs89HOuubZmSeR9NgQDyE"
      "PsMpdANRsyHZxBt1s+fS7Agdts5a716YRUDOi9MJBDfehxS7HAejkQwWmGSmO/"
      "jJEI80IUcgbst5dI8p0HBlMvpWbw5d9EXNvl2ldAJsLBbbJdmw+9msv/"
      "llbLUF7lEH1IfU68h2ilfUSEtnIYbbVQI/nUoCD1jvwI92cchJvlKu7Lxe6MuzrNFu6Bc3/"
      "IsutvOl4X3BSuHPiHbTgl92hLV4nTWr129vbJuR/"
      "a0e7njIkPmyV+236w08rHfvMPlntGHzQqQqe0nZ71yCxnXoaT7xJknnR+"
      "BSM7Zx7S1UKQ2K6T5PVPtINXXfs1sYIHW7AgMBAAECggEBAKtSpS4CtXcNcqYYUiR0eX2XHJ"
      "dbvIJaps4nkgpwP3cugjpZ+3hkt6mydKu2ULdK5zznSdNtULcXZQ/"
      "xsBtwF6sRd4W6qEX6e1DK2GaEC3Pf2x0meesHJLmXYU6JQHoCurOp7VY3dfqld87Dif4ii+"
      "xMKkfLlu20zTFcz05+f0SAt7Epizhr+N9JQRDwSVcxlm50XGoZKTNxlycZ9LfS7kvg+"
      "ISyuV1/"
      "rJTHIDvlFrROA7nyOAnhTZCr5zKHgb95jFEOQr8tjMjH63x86Ik1Pnn2r8YzQaNYhbxpNC7g"
      "pVxPXCj4D450gDGIVVA8ZqBB5+CWohgFcQxiTV6yzkjVRWECgYEA6nHawO/"
      "zenbD6jDiF3qR33Y5mGT/WrL1/"
      "BcY4fo0uWI5ptyhB0OTh9cE2VDOCUCu907DwD7KyONmfnNtvirwDt/"
      "mGWmZ6M5NDs5diRRcayt+hFT3127nyvSd9OkK062d09OieojNCWfhrGd0U209AXIXkWQDU/"
      "qIzmmvVNDOtvECgYEAxDl6yGwoR0RMxiOX+3rHY5LtEnAo/hjfVheOUyGzD/"
      "ieqpHA7XljrVsl9/UJaNWyxMOpB4ExtVWDIBaOs/5qfeBZA5Jp4vHugwhQYRbyK/"
      "px4hrzZNQFCLxPFEEHPTyhncv/0Kt8wiquk34zL6/+AUMFzleETrezlnMA5wnf72sCgYAO/"
      "g7uLaoVV8j5jfy3Yag4cRgHzdWimDUw1M1LjjOW81gaept1mnWxxydROmCiqNdHi2rrPVi8d"
      "gZU6c7HzXkiimDtmRo1kBPPcZT7siii1do2XjSnOzlpejEru0ds9/"
      "pS1ILccSkjg9gjrU38E81F7WTicFHiKfBbuthyhRPjEQKBgGPSyf/D5gm7WUNDQEITB/RD/"
      "o7uuCvsA1IM0b7A2hM5653iYD9vDaJXSE2IPq0yaatnzkmfE+aoBfkBAL/Yodw/"
      "kYkbpvppEWlfhWSfA7wTwHPCOigFJDp5cdFf84rXmXBpI07IrsMoa39lKC0OXy3NJtr98rUJ"
      "Bzd2cfMXNvDZAoGASscgcKlyHbc5hoD5GrHZb7yyE8or9MtYA1BDTG/"
      "RDJvAQOq0BrxnUxye0t6LQN449V44OH19mIBtzfM5gbTX3caw1QuFEENHnIzjMO7NGR6D1Y4"
      "HVl/Ygm7atUVKY8G0tZWIPxflzm3kbleTSlnHImcWkufoHp2laNmxEgrtLRg=\n"
      "-----END PRIVATE KEY-----\n";
  std::string privateKey2 =
      "-----BEGIN PRIVATE KEY-----\n"
      "MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDQe2TKW3uLWzZlORzRFVPb"
      "mXalH8fGzEF8BdGRS312OAEzLLtUmjTa/"
      "Yu+"
      "KtsqRz7lTtKgGnHQXfuzZQXi0Pm8c3wCwLYYewgAoek7ztkuLeYAV2hW7TMaGyTdWcR75iux"
      "0Y8L264JPOXom4pLeoEcB+EWJCIRk7TKRZ4mEjUD3t4egDQR3cYZQkCtnjeGf16c5xTSVaE/"
      "/w3F5lJtl0PbfsvxwGKNWBLNzbcFzr/J7v6wj6KuD2z5OyZbeL/eFmcqvF5AIFB/"
      "VmosqhOWU7zF/"
      "HOf4++T+arH6+"
      "YHMPiaoxZrJa05HBv282Ncwmt3fXJY0x3gmZpsu05oDrArye7rAgMBAAECggEBAKOFFRHu3S"
      "D5otmUCTkpPEPSw49SNPSFQpuPSMdoaXhvBunnnOsHpItbMN8waARqKzfaMjPnafufq4sSoq"
      "I7fJXGEq9qDal33lf4pqIFZKzmzS7ZX/pGQ72nKqv++7CeYQkwr70CSifuvvlJ/"
      "hejd17QA0jFgH8+1h+MqTV9NF3aZU8O1NstAGpnOLXMHD5VMpPHF+i3eDjN1MCXP/"
      "mC4fxesXGttYHDLiaYpXZWNWeluA4Yl1LcA0ZPD+"
      "KMrJJDzi4AlP1MxnrUdGOZp8ahRBLBZANENpcIQ7gGLQazQlb9iQ1pEzWGeyc/"
      "tAEaYW+SU5fOh2cbQwj6mQVc3brKQCkCgYEA/nNLc/"
      "gfsduknTaehibHk3r8G9Klu5EjBefBBXYN3uOnbQ+X+"
      "L9WyUV0OIYbzBhEkYGXG5TGcti3P6Q7zokNxwBZO3kivi2L+"
      "654M8pSzTP47vPP5da1ALr2md0eABNJ8A8BiISEMAcFfNPHwBbN4ZdC4JbxIqhSTmhG3XCgV"
      "WUCgYEA0cBuYyYossj5nQFe/"
      "quTMa0RW2ZLG2xxS8+u24DEr9R+"
      "OFRVm7HNrBzlieD4UrugKKBQLZisXgjHFXoxOoHhBoLAyiEejUlTG842XAr4Az9LXVMstLmk"
      "sJVRbU79xk/"
      "CX1ghpnFr26IahHHooh1ZViXXGjavWtXyjLTaa0IRVg8CgYEA4k5I3WvhRJ7FWc4qT8ZN2q2"
      "XbGd9X8VGEQp+cgz9aS+wyR8FWwdSeNB8/YjJUEyFL6TCcJVxqtN0p/"
      "W5+4eJ6WQ1+N9UNzDrP319CxL3ceq3jE7sqM69TVj72vz+"
      "obKGICw4gOLbbpvQFaWGEyI1Gl8d7AJTlS9/hJzZ6jkxYMECgYA10DcB/RJH/"
      "EP+QhHLKN3UyuA/"
      "INl4KbRPjPMBoeHUcqIHdauZDF8GH0vMugk7wKNwiODJMfhXD81z9yuWJShEcVthAGpEOWG3"
      "KorLxoym2KBPSz2XCGH2Y/jGIJP/"
      "QwtNRuErQMI5UTeYb1kN7pWzAnZTZWBL38Qg0W2QQx0LhQKBgGGUEmrAz7M8bb+"
      "qrumUmGga6Q5EkZtTA3uVf2bH6GXxKlw3TaqIfrQWMre7ov+pwx7En/"
      "ld0aeYViWHHRkMdAbdlD5KVNqYFSIYOemXf9i33L8PLxx8OEoXWmLbHhqK7VVGJiJ8JhpuB6"
      "Wkh5exMu/3oXRXlcyGykgKS5Y/Pa+P\n"
      "-----END PRIVATE KEY-----\n";

  nlohmann::json chatJson = constructor.constructChat(
      destinationServers, participants, privateMessage, publicKeys);

  std::cout << "Chat JSON: " << chatJson.dump(4) << std::endl << std::endl;

  if (reciever.validateMessage(chatJson)) {
    std::cout << "Message is valid." << std::endl;

    std::string message_type = reciever.findMessageType(chatJson);
    std::cout << "Message type is: " << message_type << std::endl << std::endl;

    nlohmann::json decryptedChat1 = reciever.decryptChat(chatJson, privateKey1);
    std::cout << "Decrypted with key 1: " << decryptedChat1.dump(4) << std::endl
              << std::endl;

    nlohmann::json decryptedChat2 = reciever.decryptChat(chatJson, privateKey2);
    std::cout << "Decrypted with key 2: " << decryptedChat2.dump(4) << std::endl
              << std::endl;
  } else {
    std::cout << "Message is invalid." << std::endl;
  }

  return 0;
}