#include "arithmetic.h"
#include "ecurve.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

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

uint16_t k2[32] = {0x57, 0x03, 0x80, 0x39, 0x3c, 0x36, 0x6c, 0xd8,
                    0x34, 0x29, 0xf3, 0xb1, 0xa9, 0x79, 0xb5, 0x93,
                    0xf0, 0x41, 0xa5, 0x80, 0x88, 0x74, 0x60, 0xc8,
                    0x16, 0x49, 0x33, 0xa0, 0x98, 0x68, 0x80, 0xc9};
uint16_t qx2[32] = {0x8f, 0x72, 0xeb, 0x65, 0xce, 0xd5, 0x18, 0x86,
                    0x7b, 0x31, 0xe0, 0x6d, 0xd0, 0x90, 0x06, 0x62,
                    0x2b, 0x7c, 0xb9, 0x1c, 0xed, 0x2f, 0xa3, 0x6b,
                    0x09, 0x80, 0xaa, 0x91, 0xc6, 0x0d, 0x72, 0xd0};
uint16_t qy2[32] = {0x5f, 0x27, 0xdc, 0x93, 0x50, 0xbc, 0x72, 0x3c,
                    0x8f, 0x6d, 0x10, 0xc7, 0xb3, 0xb1, 0xb0, 0xa9,
                    0xa9, 0xa8, 0xc4, 0xd9, 0x35, 0xd3, 0x83, 0x0d,
                    0x7d, 0xa1, 0xcd, 0xb1, 0x17, 0xb5, 0x81, 0x96};
uint16_t k3[32] = {0x6d, 0x44, 0x17, 0x82, 0xf7, 0x62, 0x36, 0xe7,
                    0xdb, 0xa2, 0x96, 0x9a, 0x6b, 0x2d, 0x12, 0x5a,
                    0x5f, 0xcc, 0x51, 0x67, 0xd6, 0x7b, 0xa9, 0x84,
                    0xc6, 0x48, 0x8f, 0x38, 0xc8, 0x35, 0x07, 0x71};
uint16_t qx3[32] = {0xb9, 0x4a, 0x9e, 0xc3, 0x65, 0x25, 0x32, 0xd2,
                    0x60, 0xbb, 0xc3, 0x6d, 0xa6, 0x24, 0xb7, 0x0e,
                    0x69, 0x80, 0x66, 0xda, 0x8d, 0x25, 0x8d, 0x2d,
                    0x18, 0xcb, 0x91, 0xdd, 0x8a, 0x6a, 0x83, 0xf6};
uint16_t qy3[32] = {0xc2, 0x9d, 0x28, 0x4b, 0x97, 0xad, 0x73, 0xc1,
                    0x72, 0xad, 0xb7, 0x4b, 0xdc, 0xed, 0x7b, 0x4e,
                    0x82, 0x1f, 0xf3, 0x2f, 0xac, 0xd0, 0xe8, 0xb8,
                    0x0c, 0x87, 0x64, 0x28, 0xa3, 0x7a, 0x83, 0x1f};
uint16_t k4[32] = {0xe4,0xf6,0x11,0x8a,0xda,0x0a,0x62,0x14,
                    0xf5,0x57,0x87,0x61,0xce,0xe2,0x61,0xff,
                    0xce,0xd8,0xec,0x3b,0xe6,0xe7,0x37,0x3e,
                    0x6b,0xc1,0xe2,0xb3,0xb7,0xd8,0xd5,0x78};
uint16_t qx4[32] = {0x4e,0x85,0x3a,0x20,0x9f,0x77,0x4f,0xe5,
                    0x03,0x9e,0x87,0x18,0xb0,0x6c,0x7b,0xd2,
                    0x88,0x4c,0xad,0x5d,0x66,0xe5,0x5f,0x6a,
                    0x4f,0xaf,0xb2,0xcb,0x26,0x11,0x71,0x76};
uint16_t qy4[32] = {0x56,0x68,0x1e,0x81,0x69,0x2b,0x45,0x52,
                    0xb3,0x09,0xa5,0xce,0xa3,0x89,0xbd,0x88,
                    0xe7,0x98,0x83,0x01,0xfd,0x20,0x36,0xfd,
                    0x48,0x52,0xab,0x60,0x99,0xf3,0x6d,0xa2};
uint16_t k5[32] = {0x8c,0xcb,0xc8,0x34,0xae,0x51,0xad,0x88,
                    0xef,0x08,0xb2,0x46,0x51,0xc2,0xc8,0x6a,
                    0x5a,0xde,0xe1,0x44,0xc2,0x20,0x74,0xe1,
                    0x5f,0x58,0x60,0x38,0x70,0xa0,0x61,0x2a};
uint16_t qx5[32] = {0xc8,0xdd,0xfc,0x66,0xd6,0x0a,0xc7,0x1e,
                    0xe7,0xc4,0xa5,0xc0,0x55,0x1b,0x7e,0xf6,
                    0x18,0xbb,0x7a,0x03,0xba,0xee,0xdd,0xaa,
                    0x88,0xc0,0xea,0xce,0x96,0x71,0xaa,0xe1};
uint16_t qy5[32] = {0xc8,0xd5,0x71,0xf9,0x54,0xdb,0x2c,0x5a,
                    0x9f,0xb5,0x33,0xa7,0x86,0xf5,0x08,0x6a,
                    0x06,0xcd,0xa9,0x27,0xcb,0x7e,0x8a,0xe9,
                    0x5d,0xdc,0xde,0xe6,0xdc,0x5b,0xd3,0xd7};
uint16_t k6[32] = {0x83,0xa8,0xf5,0xdc,0x91,0x93,0x54,0x2d,
                    0x66,0xd8,0x58,0xcb,0xb0,0xc6,0xac,0x10,
                    0x27,0x7b,0x1d,0x7f,0x07,0x1c,0x12,0x8c,
                    0xf2,0x86,0xf3,0x5f,0xb4,0x65,0xb9,0x01};
uint16_t qx6[32] = {0x10,0x8a,0x8e,0xef,0x79,0x7d,0xfc,0x48,
                    0x43,0x13,0x23,0xc4,0x3f,0x33,0x52,0x08,
                    0xe5,0xb3,0xf6,0xe8,0x15,0xb8,0x88,0x9e,
                    0xec,0x8e,0xe8,0x22,0x54,0x8c,0x03,0x1f};
uint16_t qy6[32] = {0x49,0xb8,0x89,0xc7,0x9b,0x26,0x70,0x5a,
                    0x63,0xc0,0xe0,0x62,0x29,0x0b,0x70,0xc0,
                    0x04,0xb0,0x52,0x89,0xf6,0x1e,0x36,0xfb,
                    0x4f,0x4b,0xe9,0x20,0xcb,0x47,0xa0,0x43};
uint16_t k7[32] = {0x19,0xe5,0x24,0xdc,0x3b,0x8f,0x64,0xa0,
                    0x84,0x6a,0x81,0x63,0x5a,0x1b,0x18,0x1e,
                    0x8e,0x61,0x01,0x41,0xfe,0x76,0x53,0x08,
                    0x3a,0xc5,0x74,0xd3,0x13,0x2c,0xc9,0xfa};
uint16_t qx7[32] = {0xb9,0x83,0x05,0x87,0xfa,0x72,0xbd,0x4e,
                    0xeb,0x0c,0x73,0x35,0xa6,0x8e,0x56,0x8b,
                    0x2d,0x39,0xcd,0x08,0xe3,0x33,0xcb,0x6c,
                    0xef,0x84,0xfc,0x96,0xab,0xf2,0x58,0x72};
uint16_t qy7[32] = {0xe2,0xb1,0x6b,0x2f,0xcc,0x85,0xd3,0x75,
                    0x5e,0x97,0x89,0xbe,0xcd,0xcc,0x45,0x03,
                    0x7b,0x22,0x4f,0xb9,0x69,0xfe,0xc8,0xa5,
                    0x9c,0xc2,0xbd,0x55,0xca,0x07,0x98,0x48};
uint16_t k8[32] = {0x0a,0x5a,0xef,0x2f,0x58,0xca,0x21,0x5b,
                    0x3b,0x52,0xab,0x32,0x5b,0xc4,0xa5,0x99,
                    0xf5,0xbc,0x73,0xff,0x08,0x80,0x56,0xb3,
                    0x27,0x42,0xe4,0xdd,0x92,0xa1,0x57,0xf2};
uint16_t qx8[32] = {0x72,0x78,0x99,0x68,0xc0,0x2f,0x9f,0x5b,
                    0xd2,0x27,0xe8,0x09,0xeb,0x0d,0xc9,0xa4,
                    0x40,0x60,0x60,0x4d,0xe1,0xbb,0x9b,0xb7,
                    0x12,0x55,0x7b,0x81,0x11,0x14,0xe0,0xd2};
uint16_t qy8[32] = {0x93,0xe7,0x14,0xe3,0x03,0x27,0x90,0xaa,
                    0x1e,0x84,0xe7,0xb8,0xad,0xe7,0x10,0xf7,
                    0xf7,0xfd,0xa1,0x63,0xf6,0x7f,0x50,0xc4,
                    0xf2,0x1d,0xab,0x8b,0x8f,0x13,0x3f,0x50};
uint16_t k9[32] = {0xfd,0xab,0x22,0x65,0x59,0x4c,0x13,0x46,
                    0xeb,0x76,0xfd,0x8c,0x0d,0x8e,0xa6,0x4e,
                    0x5a,0x88,0x86,0xeb,0x35,0x02,0x8f,0x70,
                    0x28,0x3d,0x2a,0xc4,0x57,0x7e,0xd6,0xad};
uint16_t qx9[32] = {0xd7,0x85,0x5f,0x17,0xf9,0x46,0xa7,0x65,
                    0xac,0xcb,0x13,0x6f,0x85,0x30,0x13,0x2b,
                    0xd5,0x4e,0x12,0xc7,0x34,0xe9,0xbd,0x30,
                    0xf2,0xb7,0x29,0xc0,0xd9,0xd2,0xbe,0x55};
uint16_t qy9[32] = {0xb0,0xe7,0x9a,0x38,0x57,0x85,0xda,0x2c,
                    0x0d,0x6b,0x2b,0x91,0x21,0x8e,0x94,0x23,
                    0x53,0xe8,0x85,0x81,0x66,0x40,0x7c,0x00,
                    0x4e,0x3b,0x58,0x1d,0x31,0x5e,0x80,0x32};



int is_equal_epoint(struct epoint* p, uint16_t* x, uint16_t* y){
    for(int i =0; i< WORD_LENGTH; i++){
        if(p->x[i]!=x[i] || p->y[i]!=y[i]){
            return 0;
        }
    }
    return 1;
}


void p256_scalar_mult_test(){
    ARITH_DEBUG = false;
    ECURVE_DEBUG = false;
    printf("P256 Point multiplication test:\n");
    struct epoint* q = epoint_init();
    struct epoint* p = epoint_init();
    for (int i=0; i<WORD_LENGTH; i++){
        p->x[i] = px[i];
        p->y[i] = py[i];
    }

    p256_scalar_mult(q, k2, p);
    print_epoint(q);
    assert(is_equal_epoint(q, qx2, qy2));

    p256_scalar_mult(q, k3, p);
    print_epoint(q);
    assert(is_equal_epoint(q, qx3, qy3));

    p256_scalar_mult(q, k4, p);
    print_epoint(q);
    assert(is_equal_epoint(q, qx4, qy4));

    p256_scalar_mult(q, k5, p);
    print_epoint(q);
    assert(is_equal_epoint(q, qx5, qy5));

    p256_scalar_mult(q, k6, p);
    print_epoint(q);
    assert(is_equal_epoint(q, qx6, qy6));

    p256_scalar_mult(q, k7, p);
    print_epoint(q);
    assert(is_equal_epoint(q, qx7, qy7));

    p256_scalar_mult(q, k8, p);
    print_epoint(q);
    assert(is_equal_epoint(q, qx8, qy8));

    p256_scalar_mult(q, k9, p);
    print_epoint(q);
    assert(is_equal_epoint(q, qx9, qy9));
}

int main(){
    p256_scalar_mult_test();
}