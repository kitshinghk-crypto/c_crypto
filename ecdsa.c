#include "ecdsa.h"
#include "ecurve.h"
#include "sha256_8u.h"
#include "arithmetic.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#define D if(ECDSA_DEBUG)

bool ECDSA_DEBUG=false;

//order=0xffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551
const uint16_t P256_ORDER[32]={0x51, 0x25, 0x63, 0xfc, 0xc2, 0xca, 0xb9, 0xf3, 
                            0x84, 0x9e, 0x17, 0xa7, 0xad, 0xfa, 0xe6, 0xbc, 
                            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                            0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff};

void static print_arr(uint8_t* rk, unsigned int len){
    for(int i =len-1; i>=0; i--){
        printf("%02x ", rk[i]);
    }
    puts("");
}

void static print_arr16(uint16_t* rk, unsigned int len){
    for(int i =len-1; i>=0; i--){
        printf("%02x ", rk[i]&0xff);
    }
    puts("");
}

void static reverse_msg(uint8_t * r_msg, const uint8_t* msg, size_t msg_byte_len){
    for(size_t i=0; i<msg_byte_len; i++){
        r_msg[msg_byte_len-i-1] = msg[i];
    }
}

void ecdsa_p256_sha256_sign(uint8_t* r, uint8_t* s, const uint8_t* d, const uint8_t* k, const uint8_t* msg, size_t mlen){
    size_t byte_len = mlen/8 + (mlen%8 > 0);
    uint8_t * r_msg = malloc(sizeof(uint8_t)*byte_len);
    uint8_t hash[32]={0};
    uint16_t k_inv[32] = {0};
    uint16_t k_16[32] = {0};
    uint16_t d_16[32] = {0};
    uint16_t h_16[32] = {0};
    struct epoint* q = epoint_init();
    struct epoint* p = p256_g();
    for(size_t i=0; i< 32; i++){
        k_16[i] = k[i];
        d_16[i] = d[i];
    }

    reverse_msg(r_msg, msg, byte_len);
    D{printf("msg:\n");print_arr(r_msg,byte_len);}
    sha256_8u_hash(hash, r_msg, mlen);
    for(size_t i=0; i< 32; i++){
        h_16[32-i-1] = hash[i];
    }
    D{printf("msg hash:\n");print_arr16(h_16,32);}
    p256_scalar_mult(q, k, p);
    D{printf("kp:");print_epoint(q);}
    inv_p(k_inv, k_16, P256_ORDER);
    D{printf("k inv:");print_arr16(k_inv,32);}
    mod_mult(d_16, q->x, P256_ORDER);
    mod_add(h_16, d_16, P256_ORDER);
    mod_mult(h_16, k_inv, P256_ORDER);
    for(size_t i=0; i< 32; i++){
        s[i] = h_16[i] & 0xff;
        r[i] = q->x[i] & 0xff;
    }
    D{printf("r:");print_arr(r,32);}
    D{printf("s:");print_arr(s,32);}
}