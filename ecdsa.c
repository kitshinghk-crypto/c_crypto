#define WORD_LENGTH 32
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

void static print_arr(const uint8_t* rk, unsigned int len){
    for(int i =len-1; i>=0; i--){
        printf("%02x ", rk[i]);
    }
    puts("");
}

/*
int static is_equal(const uint16_t* x,const uint16_t* y){
    for(int i =0; i<32; i++){
        if(x[i]!=y[i]){
            return 0;
        }
    }
    return 1;
}*/

void static print_arr16(const uint16_t* rk, unsigned int len){
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
    p256_scalar_mult(q, k, p, 0);
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
    free(r_msg);r_msg=0;
    free(p);p=0;
    free(q);q=0;
}

int ecdsa_p256_sha256_verify(const uint8_t* r, const uint8_t* s, const uint8_t* qx, const uint8_t* qy,  
                                const uint8_t* msg, size_t mlen){
    size_t byte_len = mlen/8 + (mlen%8 > 0);
    uint8_t * r_msg = malloc(sizeof(uint8_t)*byte_len);
    uint8_t hash[32]={0};
    uint16_t h_16[32] = {0};
    uint16_t s_inv[32] = {0};
    uint16_t s_16[32] = {0};
    uint16_t r_16[32] = {0};
    uint16_t u1[32] = {0};
    uint16_t u2[32] = {0};
    uint8_t u1_8[32] = {0};
    uint8_t u2_8[32] = {0};
    struct epoint* q1 = epoint_init();
    struct epoint* p1 = p256_g();
    struct epoint* q2 = epoint_init();
    struct epoint* p2 = epoint_init();
    for(size_t i=0; i<32; i++){
        p2->x[i] = qx[i];
        p2->y[i] = qy[i];
    }
    reverse_msg(r_msg, msg, byte_len);
    D{printf("msg:\n");print_arr(r_msg,byte_len);}
    sha256_8u_hash(hash, r_msg, mlen);
    for(size_t i=0; i< 32; i++){
        h_16[32-i-1] = hash[i];
    }
    D{printf("msg hash:\n");print_arr16(h_16,32);}
    for(size_t i=0; i< 32; i++){
        u1[i] = h_16[i]; u2[i] = r[i];s_16[i]=s[i];r_16[i]=r[i];
    }
    inv_p(s_inv, s_16, P256_ORDER);
    D{printf("s inv:");print_arr16(s_inv,32);}
    mod_mult(u1, s_inv, P256_ORDER);
    D{printf("u1:");print_arr16(u1,32);}
    mod_mult(u2, s_inv, P256_ORDER);
    D{printf("u2:");print_arr16(u2,32);}
    for(size_t i=0; i<32; i++){
        u1_8[i] = u1[i] & 0xff;
        u2_8[i] = u2[i] & 0xff;
    }
    p256_scalar_mult(q1,u1_8,p1,0);
    D{printf("Epoint q1:\n");print_epoint(q1);}
    p256_scalar_mult(q2,u2_8,p2,0);
    D{printf("Epoint q2:\n");print_epoint(q2);}
    struct epoint_proj* q3 = epoint_convert_proj(q1);
    D{printf("Epoint q3:\n");print_epoint_proj(q3);}
    p256_point_add(q3,q2);
    struct epoint* qa3 = p256_proj_to_affine(q3);

    D{printf("expected r:\n"); print_arr(r, 32);}
    D{printf("actual r:\n"); print_arr16(qa3->x, 32);}
    
    uint8_t result =is_equal(r_16, qa3->x);
    free(p1);free(q1);free(p2);free(q2);free(q3);free(qa3);free(r_msg);
    p1=0;q1=0;p2=0;q2=0;q3=0;qa3=0;r_msg=0;
    return result;
}