#include <iostream>
#include <string.h>

#include "encrypt.h"

// ENCRYPT THE TEXT FROM USER //
int encrypt(const unsigned char* text, int text_len, const unsigned char* key, unsigned char* cipher)
{
    int cipher_len = 0;
    int len = 0;
    
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        perror("EVP_CIPHER_CTX_new() failed");
        exit(-1);
    }

    if (!EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
    {
        perror("EVP_EncryptInit_ex() failed");
        exit(-1);
    }

    if (!EVP_EncryptUpdate(ctx, cipher, &len, text, text_len))
    {
        perror("EVP_EncryptUpdate() failed");
        exit(-1);
    }

    cipher_len += len;

    if (!EVP_EncryptFinal_ex(ctx, cipher + len, &len))
    {
        perror("EVP_EncryptFinal_ex failed");
        exit(-1);
    }

    cipher_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return cipher_len;
}

// DECRYPT THE ENCRYPTED TEXT //
int decrypt(unsigned char* cipher, int cipher_len, unsigned char* key, unsigned char* text)
{
    int text_len = 0;
    int len = 0;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        perror("EVP_CIPHER_CTX_new() failed");
        exit(-1);
    }

    if(!EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
    {
        perror("EVP_DecryptInit_ex() failed");
        exit(-1);
    }

    if (!EVP_DecryptUpdate(ctx, text, &len, cipher, cipher_len))
    {
        perror("EVP_DecryptUpdate() failed");
        exit(-1);
    }

    text_len += len;

    if (!EVP_DecryptFinal(ctx, text + len, &len))
    {
        perror("EVP_DecryptFinal() failed");
        exit(-1);
    }

    text_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return text_len;
}

// CALLING FUNCTION: TO HANDLE USER PASSWORD TO BE ENCRYPTED AND RETURN THE RESULT //
std::string hash_password(std::string text)
{
    // Key to cipher the text
    unsigned char* key = (unsigned char*)"0123456789abcdef";
    // Len of text of user password
    int text_len = text.size();
    // Convert string to unsigned char for encrypt function
    const unsigned char* new_text = reinterpret_cast<const unsigned char*>(text.c_str());
    
    // Encrypt the text and store the characters in array
    unsigned char cipher[64];
    int cipher_len = encrypt(new_text, text_len, key, cipher);    

    // The encrypted code store in cipher array is store in unsigned char,
    // Which basically hexadecimal and can only printed in hexadecimal,
    // In order to store it in database, we need to convert the hex to a string for better read
    std::string str = convert_hex_to_string(cipher, cipher_len);

    return str;
}

// FUNCTION TO CONVERT HEXADECIMAL TO STRING //
std::string convert_hex_to_string(unsigned char* cipher, int cipher_len)
{
    // target buffer should be large two times than cipher_len //
    // Because each char produce 2 hex value //
    int size = 2 * cipher_len;
    char* str = new char[size];

    unsigned char* hex = (unsigned char*)"0123456789abcdef";
    unsigned char* pin = cipher;
    char* pout = str;

    // Iterate through encrypted code to convert to string
    for(; pin < cipher + cipher_len; pout += 2, pin++)
    {
        pout[0] = hex[(*pin>>4) & 0xF];
        pout[1] = hex[ *pin     & 0xF];
        if (pout + 2 - str > size)
        {
            /* Better to truncate output string than overflow buffer */
            /* it would be still better to either return a status */
            /* or ensure the target buffer is large enough and it never happen */
            break;
        }
    }

    // Convert char[] that store converted hex to string
    std::string result(str);
    // // This is to remove remaining unsigned char that is not converted at very last position
    // result.erase(result.end() - 1, result.end() - 0);

    delete[] str;
    
    return result; 
}
