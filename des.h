#ifndef DES_H
#define DES_H
#include <stdint.h>
typedef int bool;
#define true 1
#define false 0

extern bool DES_DEBUG;
extern void des_encrypt(uint8_t*, const uint8_t*, const uint8_t*);
extern void des_decrypt(uint8_t*, const uint8_t*, const uint8_t*);
#endif