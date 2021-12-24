#ifndef SHA256_H
#define SHA256_H
typedef int bool;
#define true 1
#define false 0
#include <stdint.h>
#include <stddef.h>

extern bool SHA256_DEBUG;

extern int sha256_hash(uint32_t* hash, uint8_t* m, size_t len );

#endif