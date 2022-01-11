#ifndef RSA_H
#define RSA_H
#include <stdint.h>
#include <stddef.h>
typedef int bool;
#define true 1
#define false 0
extern bool RSA_DEBUG;

extern void pkcs1_15_sha256_sig(uint8_t* s, const uint8_t* m, const uint8_t* n, const uint8_t* d, size_t mlen);
#endif
