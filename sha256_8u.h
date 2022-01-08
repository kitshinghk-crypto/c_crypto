#ifndef SHA256_8U_H
#define SHA256_8U_H
typedef int bool;
#define true 1
#define false 0
#include <stdint.h>
#include <stddef.h>

extern bool SHA256_8U_DEBUG;

extern void sha256_8u_hash(uint8_t* hash, const uint8_t* m, size_t len );

#endif