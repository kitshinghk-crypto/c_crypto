#define WORD_LENGTH 32
#include "ecdh.h"
#include "ecurve.h"
#include "arithmetic.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#define D if(ECDH_DEBUG)

bool ECDH_DEBUG=false;

void ecdh_p256(uint8_t* k, const uint8_t* d, const uint8_t* qx, const uint8_t* qy){
    uint16_t qx16[32] = {0};
    uint16_t qy16[32] = {0};
    struct epoint* p = epoint_init();
    struct epoint* q = epoint_init();
    for(size_t i=0; i<32; i++){
        p->x[i] = qx[i]; p->y[i] = qy[i];
    }
    p256_scalar_mult(q, d, p, 0);
    for(size_t i=0; i<32; i++){
        k[i] = q->x[i] & 0xff;
    }
}