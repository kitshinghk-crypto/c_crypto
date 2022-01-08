#ifndef ECDSA_H
#define ECDSA_H
#include <stdint.h>
#include <stddef.h>
typedef int bool;
#define true 1
#define false 0
extern bool ECDSA_DEBUG;

extern void ecdsa_p256_sha256_sign(uint8_t* r, uint8_t* s, const uint8_t* d, const uint8_t* k, const uint8_t* msg, size_t mlen);
#endif