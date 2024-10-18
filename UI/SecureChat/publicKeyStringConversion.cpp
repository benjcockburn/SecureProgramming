//Bailey Atkinson (a1772413), Ben Cockburn (a1850196), Marcus Beckwith (a1852054)

#include "publicKeyStringConversion.h"

/* public key conversion */
std::string rsaPublicKeyToString(RSA* publicKey) {
  BIO* bio = BIO_new(BIO_s_mem());
  PEM_write_bio_RSA_PUBKEY(bio, publicKey);

  char* pemKey = nullptr;
  long keyLength = BIO_get_mem_data(bio, &pemKey);

  std::string publicKeyString(pemKey, keyLength);

  BIO_free(bio);
  return publicKeyString;
}

RSA* stringToRsaPublicKey(const std::string& publicKey) {
  BIO* bio = BIO_new_mem_buf(publicKey.c_str(), -1);
  RSA* publicKeyRsa = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);

  BIO_free(bio);
  return publicKeyRsa;
}

/* private key conversion */
RSA* stringToRsaPrivateKey(const std::string& privateKeyString) {
  BIO* bio = BIO_new_mem_buf(privateKeyString.c_str(), -1);
  RSA* privateKeyRsa = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL);

  BIO_free(bio);
  return privateKeyRsa;
}
