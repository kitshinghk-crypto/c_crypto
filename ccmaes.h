#ifndef CCMAES_H
#define CCMAES_H
#include <stdint.h>
#include <stddef.h>
typedef int bool;
#define true 1
#define false 0

extern bool CCMAES_DEBUG;
extern void ccm_aes_encrypt(uint8_t* cipher, const uint8_t* plaintext, const uint8_t* key, const uint8_t* nonce,
                            const uint8_t* a, uint8_t nlen, size_t alen, size_t plen, uint8_t tlen);
extern int ccm_aes_decrypt(uint8_t* plaintext, const uint8_t* cipher, const uint8_t* key, const uint8_t* nonce,
                            const uint8_t* a, uint8_t nlen, size_t alen, size_t clen, uint8_t tlen);
#endif