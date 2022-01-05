#include "arithmetic.h"
#include "ecurve.h"
#include <stdlib.h>
#include <stdio.h>

//px=0x6b 17 d1 f2 e1 2c 42 47 f8 bc e6 e5 63 a4 40 f2 77 03 7d 81 2d eb 33 a0 f4 a1 39 45 d8 98 c2 96
uint16_t px[32] ={0x96, 0xc2, 0x98, 0xd8, 0x45, 0x39, 0xa1, 0xf4,
                0xa0, 0x33, 0xeb, 0x2d, 0x81, 0x7d, 0x03, 0x77,
                0xf2, 0x40, 0xa4, 0x63, 0xe5, 0xe6, 0xbc, 0xf8,
                0x47, 0x42, 0x2c, 0xe1, 0xf2, 0xd1, 0x17, 0x6b};

//py=0x4f e3 42 e2 fe 1a 7f 9b 8e e7 eb 4a 7c 0f 9e 16 2b ce 33 57 6b 31 5e ce cb b6 40 68 37 bf 51 f5
uint16_t py[32] ={0xf5, 0x51, 0xbf, 0x37, 0x68, 0x40, 0xb6, 0xcb,
                0xce, 0x5e, 0x31, 0x6b, 0x57, 0x33, 0xce, 0x2b,
                0x16, 0x9e, 0x0f, 0x7c, 0x4a, 0xeb, 0xe7, 0x8e,
                0x9b, 0x7f, 0x1a, 0xfe, 0xe2, 0x42, 0xe3, 0x4f};

//x1: 7c f2 7b 18 8d 03 4f 7e 8a 52 38 03 04 b5 1a c3 c0 89 69 e2 77 f2 1b 35 a6 0b 48 fc 47 66 99 78
uint16_t p2x[32] = {0x78, 0x99, 0x66, 0x47, 0xfc, 0x48, 0x0b, 0xa6,
                    0x35, 0x1b, 0xf2, 0x77, 0xe2, 0x69, 0x89, 0xc0,
                    0xc3, 0x1a, 0xb5, 0x04, 0x03, 0x38, 0x52, 0x8a,
                    0x7e, 0x4f, 0x03, 0x8d, 0x18, 0x7b, 0xf2, 0x7c};
//y1: 07 77 55 10 db 8e d0 40 29 3d 9a c6 9f 74 30 db ba 7d ad e6 3c e9 82 29 9e 04 b7 9d 22 78 73 d1
uint16_t p2y[32] = {0xd1, 0x73, 0x78, 0x22, 0x9d, 0xb7, 0x04, 0x9e,
                    0x29, 0x82, 0xe9, 0x3c, 0xe6, 0xad, 0x7d, 0xba,
                    0xdb, 0x30, 0x74, 0x9f, 0xc6, 0x9a, 0x3d, 0x29,
                    0x40, 0xd0, 0x8e, 0xdb, 0x10, 0x55, 0x77, 0x07};

void p256_double_test(){
    ARITH_DEBUG = false;
    ECURVE_DEBUG = false;
    printf("P256 Point Double test:\n");
    struct epoint_proj* p = epoint_proj_init();
    for (int i=0; i<WORD_LENGTH; i++){
        p->x[i] = px[i];
        p->y[i] = py[i];
    }
    p->z[0] = 1;
    p256_point_double(p);
    print_epoint_proj(p);
}

void p256_add_test(){
    ARITH_DEBUG = false;
    ECURVE_DEBUG = false;
    printf("P256 Point Addition test:\n");
    struct epoint_proj* p = epoint_proj_init();
    struct epoint* q = epoint_init();
    for (int i=0; i<WORD_LENGTH; i++){
        p->x[i] = p2x[i];
        p->y[i] = p2y[i];
        q->x[i] = px[i];
        q->y[i] = py[i];
    }
    p->z[0] = 1;
    p256_point_add(p,q);
    print_epoint_proj(p);
}

void P256_inv_test(){
    ARITH_DEBUG = false;
    uint16_t P256[32] ={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
                    0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                    0x01,0x00,0x00,0x00,0xff,0xff,0xff,0xff};
    uint16_t inv[32] = {0};
    uint16_t two[32] = {2};
    inv_p(inv, two, P256);
    printf("inverse test:\n");
    print_hex(inv, 32); 
}

int main(){
    p256_double_test();
    p256_add_test();
    P256_inv_test();
}