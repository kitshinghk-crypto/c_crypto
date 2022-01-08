#include "arithmetic.h"
#include "ecurve.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#define D if(ECURVE_DEBUG)

bool ECURVE_DEBUG = false;
//p=0xff ff ff ff 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 00 ff ff ff ff ff ff ff ff ff ff ff ff
const uint16_t P256[32] ={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
                    0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                    0x01,0x00,0x00,0x00,0xff,0xff,0xff,0xff};

//px=0x6b 17 d1 f2 e1 2c 42 47 f8 bc e6 e5 63 a4 40 f2 77 03 7d 81 2d eb 33 a0 f4 a1 39 45 d8 98 c2 96
const uint8_t P256_GX[32] ={0x96, 0xc2, 0x98, 0xd8, 0x45, 0x39, 0xa1, 0xf4,
                0xa0, 0x33, 0xeb, 0x2d, 0x81, 0x7d, 0x03, 0x77,
                0xf2, 0x40, 0xa4, 0x63, 0xe5, 0xe6, 0xbc, 0xf8,
                0x47, 0x42, 0x2c, 0xe1, 0xf2, 0xd1, 0x17, 0x6b};

//py=0x4f e3 42 e2 fe 1a 7f 9b 8e e7 eb 4a 7c 0f 9e 16 2b ce 33 57 6b 31 5e ce cb b6 40 68 37 bf 51 f5
const uint8_t P256_GY[32] ={0xf5, 0x51, 0xbf, 0x37, 0x68, 0x40, 0xb6, 0xcb,
                0xce, 0x5e, 0x31, 0x6b, 0x57, 0x33, 0xce, 0x2b,
                0x16, 0x9e, 0x0f, 0x7c, 0x4a, 0xeb, 0xe7, 0x8e,
                0x9b, 0x7f, 0x1a, 0xfe, 0xe2, 0x42, 0xe3, 0x4f}; 

//2inv=0x7f ff ff ff 80 00 00 00 80 00 00 00 00 00 00 00 00 00 00 00 80 00 00 00 00 00 00 00 00 00 00 00
const uint16_t P256_2_inv[32]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                        0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,
                        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
                        0x00,0x00,0x00,0x80,0xff,0xff,0xff,0x7f};

void print_epoint(const struct epoint* p){
    printf("Print Epoint\n");
    printf("X:");
    print_hex(p->x, WORD_LENGTH);
    printf("Y:");
    print_hex(p->y, WORD_LENGTH);
}

void print_epoint_proj(const struct epoint_proj* p){
    printf("Print Epoint\n");
    printf("X:");
    print_hex(p->x, WORD_LENGTH);
    printf("Y:");
    print_hex(p->y, WORD_LENGTH);
    printf("Z:");
    print_hex(p->z, WORD_LENGTH);
}

struct epoint* p256_g(){
    struct epoint* p = epoint_init();
    for(size_t i=0; i<32; i++){
        p->x[i] = P256_GX[i];
        p->y[i] = P256_GY[i];
    }
    return p;
}

struct epoint* epoint_init(){
    struct epoint* p = malloc(sizeof(struct epoint));
    p->x = malloc(sizeof(uint16_t)*WORD_LENGTH);
    p->y = malloc(sizeof(uint16_t)*WORD_LENGTH);
    for(uint8_t i =0; i< WORD_LENGTH; i++){
        p->x[i] = 0;
        p->y[i] = 0;
    }
    return p;
}

struct epoint_proj* epoint_proj_init(){
    struct epoint_proj* pj = malloc(sizeof(struct epoint_proj));
    pj->x = malloc(sizeof(uint16_t)*WORD_LENGTH);
    pj->y = malloc(sizeof(uint16_t)*WORD_LENGTH);
    pj->z = malloc(sizeof(uint16_t)*WORD_LENGTH);
    for(uint8_t i =0; i< WORD_LENGTH; i++){
        pj->x[i] = 0;
        pj->y[i] = 0;
        pj->z[i] = 0;
    }
    return pj;
}

struct epoint_proj* epoint_convert_proj(const struct epoint* p){
    struct epoint_proj* pj = epoint_proj_init();
    copy(pj->x, p->x);
    copy(pj->y, p->y);
    for(uint8_t i =0; i< WORD_LENGTH; i++){
        pj->z[i] = 0;
    }
    pj->z[0] = 1;
    return pj;
}

void set_epoint_inf(const struct epoint_proj* p){
    for(int i =0; i< WORD_LENGTH;i++){
        p->x[i] =0; p->y[i]=0; p->z[i] =0;
    }
    p->x[0]=1; p->y[0]=1;
}

int is_epoint_inf(const struct epoint_proj* p){
    return is_one(p->x) && is_one(p->y) && is_zero(p->z);
}

/**
**  Ref: Guide to elliptic curve cryptography 
**  Author: Hankerson, D., Menezes, A.J. and Vanstone, S.
**  Page: 91, Algorithm 3.21 Point doubling (y2 = x3 − 3x + b, Jacobian coordinates)
**  input: a,p
**  output: a^-1 mod p
**  return: inv
**/
int p256_point_double(struct epoint_proj* p){
    D{printf("Point Double:\n");print_epoint_proj(p);}
    if(is_epoint_inf(p)){
        D{printf("Point at inf:\n");}
        return 1;
    }
    uint16_t t1[WORD_LENGTH] = {0};
    uint16_t t2[WORD_LENGTH] = {0};
    uint16_t t3[WORD_LENGTH] = {0};
    uint16_t x3[WORD_LENGTH] = {0};
    uint16_t y3[WORD_LENGTH] = {0};
    uint16_t z3[WORD_LENGTH] = {0};
    uint16_t c[WORD_LENGTH] = {0};
    for(int i=0; i< WORD_LENGTH; i++){
        t1[i]=p->z[i];t2[i]=p->x[i];
    }
    //t1=k(pz^2)
    mod_mult(t1,p->z,P256);
    D{printf("t1:\n");print_hex(t1, WORD_LENGTH);}
    //t2=k(px-t1)
    mod_sub(t2,t1,P256);
    D{printf("t2:\n");print_hex(t2, WORD_LENGTH);}
    //t1=k(px+t1)
    mod_add(t1, p->x, P256);
    D{printf("t1:\n");print_hex(t1, WORD_LENGTH);}
    //t2=k(t2*t1)
    mod_mult(t2,t1,P256);
    D{printf("t2:\n");print_hex(t2, WORD_LENGTH);}
    //t2=k(3*t2)
    c[0]=3;
    mod_mult(t2,c,P256);
    D{printf("t2:\n");print_hex(t2, WORD_LENGTH);}
    //y3=k(2*py)
    y3[0]=2;
    mod_mult(y3,p->y,P256);
    D{printf("y3:\n");print_hex(y3, WORD_LENGTH);}
    //z3=k(y3*pz)
    copy(z3, y3);
    mod_mult(z3,p->z,P256);
    D{printf("z3:\n");print_hex(z3, WORD_LENGTH);}
    //y3=k((y3)^2)
    mod_mult(y3,y3,P256);
    D{printf("y3:\n");print_hex(y3, WORD_LENGTH);}
    //t3=k(y3*px)
    copy(t3,y3);
    mod_mult(t3,p->x, P256);
    D{printf("t3:\n");print_hex(t3, WORD_LENGTH);}
    //y3=k((y3)^2)
    mod_mult(y3,y3,P256);
    D{printf("y3:\n");print_hex(y3, WORD_LENGTH);}
    //y3=k(y3/2)
    mod_mult(y3,P256_2_inv,P256);
    D{printf("y3:\n");print_hex(y3, WORD_LENGTH);}
    //x3=k((t2)^2)
    copy(x3, t2);
    mod_mult(x3,x3,P256);
    D{printf("x3:\n");print_hex(x3, WORD_LENGTH);}
    //t1=k(2*t3)
    c[0]=2;
    copy(t1,t3);
    mod_mult(t1,c,P256);
    D{printf("t1:\n");print_hex(t1, WORD_LENGTH);}
    //x3=k(x3-t1)
    mod_sub(x3,t1,P256);
    D{printf("x3:\n");print_hex(x3, WORD_LENGTH);}
    //t1=k(t3-x3)
    copy(t1, t3);
    mod_sub(t1,x3,P256);
    D{printf("t1:\n");print_hex(t1, WORD_LENGTH);}
    //t1=k(t1*t2)
    mod_mult(t1,t2,P256);
    D{printf("t1:\n");print_hex(t1, WORD_LENGTH);}
    //y3=k(t1-y3)
    copy(t2, t1);
    mod_sub(t2,y3,P256);
    copy(y3,t2);
    D{printf("y3:\n");print_hex(y3, WORD_LENGTH);}

    copy(p->x, x3);
    copy(p->y, y3);
    copy(p->z, z3);

    //free(t1);free(t2);free(t3);free(x3);free(y3);free(z3);free(c);
    //t1=NULL;t2=NULL;t3=NULL;x3=NULL;y3=NULL;z3=NULL;c=NULL;
    return 1;
}

/**
**  Ref: Guide to elliptic curve cryptography 
**  Author: Hankerson, D., Menezes, A.J. and Vanstone, S.
**  Page: 91, Algorithm 3.22 Point addition (y2 = x3 − 3x + b, affine-Jacobian coordinates)
**  input: a,p
**  output: a^-1 mod p
**  return: inv
**/
int p256_point_add(struct epoint_proj* p, const struct epoint* q){
    D{printf("Point Addition:\n");print_epoint_proj(p);}
    D{print_epoint(q);}
    if(is_epoint_inf(p)){
        D{printf("Point P at inf:\n");}
        copy(p->x, q->x);
        copy(p->y, q->y);
        for(int i=0; i<WORD_LENGTH; i++){
            p->z[i] =0;
        }
        p->z[0] =1;
        return 1;
    }
    uint16_t t1[WORD_LENGTH] = {0};
    uint16_t t2[WORD_LENGTH] = {0};
    uint16_t t3[WORD_LENGTH] = {0};
    uint16_t t4[WORD_LENGTH] = {0};
    uint16_t x3[WORD_LENGTH] = {0};
    uint16_t y3[WORD_LENGTH] = {0};
    uint16_t z3[WORD_LENGTH] = {0};
    uint16_t c[WORD_LENGTH]= {0};
    for(int i=0; i< WORD_LENGTH; i++){
        t1[i]=p->z[i];t2[i]=p->z[i];z3[i]=p->z[i];
    }
    //t1=k((z1)^2)
    mod_mult(t1,t1,P256);
    D{printf("t1:\n");print_hex(t1, WORD_LENGTH);}
    //t2=k(t1*z1)
    mod_mult(t2,t1,P256);
    D{printf("t2:\n");print_hex(t2, WORD_LENGTH);}
    //t1=k(t1*x2)
    mod_mult(t1,q->x,P256);
    D{printf("t1:\n");print_hex(t1, WORD_LENGTH);}
    //t2=k(t2*y2)
    mod_mult(t2,q->y,P256);
    D{printf("t2:\n");print_hex(t2, WORD_LENGTH);}
    //t1=k(t1-x1)
    mod_sub(t1,p->x,P256);
    D{printf("t1:\n");print_hex(t1, WORD_LENGTH);}
    //t2=k(t2-y1)
    mod_sub(t2,p->y,P256);
    D{printf("t2:\n");print_hex(t2, WORD_LENGTH);}
    if(is_zero(t1)){
        if(is_zero(t2)){
            D{printf("P=Q, compute 2P\n");}
            copy(p->x, q->x);
            copy(p->y, q->y);
            for(int i=0; i<WORD_LENGTH; i++){
                p->z[i] =0;
            }
            p->z[0] =1;
            p256_point_double(p);
        }else{
             D{printf("Q=-P, return infinity\n");}
            for(int i =0; i< WORD_LENGTH;i++){
                p->x[i] = 0;p->y[i] = 0;p->z[i] = 0;
            }
            p->x[0] =1;p->y[0] = 1;
        }
        return 1;
    }
    //z3=k(z1*t1)
    mod_mult(z3,t1,P256);
    D{printf("z3:\n");print_hex(z3, WORD_LENGTH);}
    //t3=k((t1)^2)
    copy(t3, t1);
    mod_mult(t3,t3,P256);
    D{printf("t3:\n");print_hex(t3, WORD_LENGTH);}
    //t4=k(t3*t1)
    copy(t4, t3);
    mod_mult(t4,t1,P256);
    D{printf("t4:\n");print_hex(t4, WORD_LENGTH);}
    //t3=k(t3*x1)
    mod_mult(t3,p->x,P256);
    D{printf("t3:\n");print_hex(t3, WORD_LENGTH);}
    //t1=k(2*t3)
    c[0]=2;
    copy(t1,t3);
    mod_mult(t1,c,P256);
    D{printf("t1:\n");print_hex(t1, WORD_LENGTH);}
    //x3=k((t2)^2)
    copy(x3, t2);
    mod_mult(x3,t2,P256);
    D{printf("x3:\n");print_hex(x3, WORD_LENGTH);}
    //x3=k(x3-t1)
    mod_sub(x3,t1,P256);
    D{printf("x3:\n");print_hex(x3, WORD_LENGTH);}
    //x3=k(x3-t4)
    mod_sub(x3,t4,P256);
    D{printf("x3:\n");print_hex(x3, WORD_LENGTH);}
    //t3=k(t3-x3)
    mod_sub(t3,x3,P256);
    D{printf("t3:\n");print_hex(t3, WORD_LENGTH);}
    //t3=k(t3*t2)
    mod_mult(t3,t2,P256);
    D{printf("t3:\n");print_hex(t3, WORD_LENGTH);}
    //t4=k(t4*y1)
    mod_mult(t4,p->y,P256);
    D{printf("t4:\n");print_hex(t4, WORD_LENGTH);}
    //y3=k(t3-t4)
    copy(y3,t3);
    mod_sub(y3,t4,P256);
    D{printf("y3:\n");print_hex(y3, WORD_LENGTH);}

    copy(p->x, x3);
    copy(p->y, y3);
    copy(p->z, z3);
    return 1;
}

struct epoint* p256_proj_to_affine(struct epoint_proj* p){
    struct epoint* ap = epoint_init();
    uint16_t t[WORD_LENGTH] = {0};
    uint16_t z2inv[WORD_LENGTH] = {0};
    uint16_t z3inv[WORD_LENGTH] = {0};
    copy(t, p->z);
    mod_mult(t,p->z,P256);
    D{printf("z^2\n"); print_hex(t, WORD_LENGTH);}
    inv_p(z2inv,t,P256);
    D{printf("z^-2\n"); print_hex(z2inv, WORD_LENGTH);}
    mod_mult(t,p->z,P256);
    D{printf("z^3\n"); print_hex(t, WORD_LENGTH);}
    inv_p(z3inv,t,P256);
    D{printf("z^-3\n"); print_hex(z3inv, WORD_LENGTH);}
    copy(ap->x, p->x);
    copy(ap->y, p->y);
    mod_mult(ap->x, z2inv, P256);
    mod_mult(ap->y, z3inv, P256);
    return ap;
}


int p256_scalar_mult(struct epoint* kp, const uint8_t* k, const struct epoint* p){
    struct epoint_proj* q = epoint_proj_init();
    set_epoint_inf(q);
    for(int i=255;i>=0;i--){
        uint8_t bit = (k[i/8] >> (i%8)) & 1U;
        p256_point_double(q);
        if(bit == 1){
            p256_point_add(q,p);
        }
    }
    struct epoint* ap  = p256_proj_to_affine(q);
    copy(kp->x, ap->x);
    copy(kp->y, ap->y);
    return 1;
}