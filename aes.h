#ifndef DES_H
#define DES_H
typedef int bool;
#define true 1
#define false 0
#include <stdint.h>
#include <stddef.h>
extern bool AES_DEBUG;
extern void aes_encrypt(uint8_t* cipher, const uint8_t* key, const uint8_t* plaintext);
extern void aes_decrypt(uint8_t* plaintext, const uint8_t* key, const uint8_t* cipher);
#endif