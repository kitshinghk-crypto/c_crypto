#ifndef ECDH_H
#define ECDH_H
#include <stdint.h>
#include <stddef.h>
typedef int bool;
#define true 1
#define false 0
extern bool ECDH_DEBUG;

extern void ecdh_p256(uint8_t* k, const uint8_t* d, const uint8_t* qx, const uint8_t* qy);
#endif