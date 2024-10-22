#ifndef encrypt_h
#define encrypt_h

#include <openssl/evp.h>
#include <openssl/conf.h>

int encrypt(const unsigned char* text, int text_len, const unsigned char* key, unsigned char* cipher);
int decrypt(unsigned char* cipher, int cipher_len, unsigned char* key, unsigned char* text);
std::string hash_password(std::string text);
std::string convert_hex_to_string(unsigned char* cipher, int cipher_len);

#endif