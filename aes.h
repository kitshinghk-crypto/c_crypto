#ifndef DES_H
#define DES_H
typedef int bool;
#define true 1
#define false 0
#include <stdint.h>
#include <stddef.h>
extern bool AES_DEBUG;
extern int aes_encrypt(uint8_t* cipher, const uint8_t* key, const uint8_t* plaintext);
extern int aes_decrypt(uint8_t* plaintext, const uint8_t* key, const uint8_t* cipher);
//debug
//extern int init_key_schedule(uint8_t* roundkey, const uint8_t* key);
//extern void printBits(size_t const , void const * const );
//extern void print_arr(unsigned char* rk, unsigned int len);
#endif