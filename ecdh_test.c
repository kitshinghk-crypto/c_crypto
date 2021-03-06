#include "ecdh.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <assert.h>

int static is_equal(uint8_t* k, uint8_t* expected_k){
    for(int i =0; i< 32; i++){
        if(k[i]!=expected_k[i]){
            return 0;
        }
    }
    return 1;
}

/*
    uint8_t qx[32] = {};
    uint8_t qy[32] = {};
    uint8_t d[32] = {};
    uint8_t expected_k[32] = {};
    uint8_t k[32] = {0};
    ecdh_p256(k, d, qx, qy);
    assert(is_equal(k, expected_k));
    printf("PASS ECDH test \n");
    */
int main(){
    uint8_t qx1[32] = {0x87,0xd2,0x33,0x88,0x83,0xcc,0xe7,0x2c,0xb4,0xf6,0x4d,0x3a,0xce,0xac,0x6b,0x1b,0xb9,0x0d,0x64,0x65,0xca,0x32,0xc6,0x5c,0x4c,0x58,0x56,0x7f,0xf7,0x48,0x0c,0x70};
    uint8_t qy1[32] = {0xac,0xa4,0x5f,0xb8,0xca,0x82,0x17,0x44,0xe0,0xdf,0x40,0xf6,0xfb,0x46,0x8d,0x94,0xc5,0xdc,0x51,0x5c,0xba,0x20,0xdb,0x0d,0x06,0x9b,0xfd,0xe3,0x09,0xe5,0x71,0xdb};
    uint8_t d1[32] = {0x34,0xa5,0xc1,0x2b,0xb6,0xad,0x0b,0xd8,0x2e,0xd2,0xb6,0x1f,0xaf,0x58,0x90,0x3d,0xe0,0xea,0x2e,0x63,0x14,0x62,0x0d,0xf8,0xda,0x9d,0xb2,0x1e,0xf7,0xc5,0x7d,0x7d};
    uint8_t expected_k1[32] = {0x7b,0xbd,0x97,0x89,0x77,0xd7,0x0d,0x04,0x68,0x1e,0x56,0x60,0x20,0x85,0xc5,0xcc,0x25,0x2d,0xdd,0xfb,0x34,0xa4,0x54,0x2e,0x01,0xff,0x20,0x64,0x10,0x62,0xfc,0x46};
    uint8_t k1[32] = {0};
    ecdh_p256(k1, d1, qx1, qy1);
    assert(is_equal(k1, expected_k1));
    printf("PASS ECDH test 1\n");

    uint8_t qx2[32] = {0xc3,0xfe,0xe7,0x0e,0x44,0x36,0x36,0x44,0x77,0x4d,0x46,0x8b,0x45,0xbf,0xde,0x67,0xd6,0x4a,0x8b,0x26,0x33,0xe5,0x6d,0x54,0x33,0x3c,0xa0,0xd4,0x12,0x9c,0x33,0xa2,};
    uint8_t qy2[32] = {0x36,0xf5,0xfa,0x66,0xcb,0x73,0xbf,0x97,0xa4,0x69,0x28,0x96,0xae,0x8e,0xe8,0xda,0x39,0x68,0x07,0x51,0x18,0x2c,0xda,0xbc,0x20,0x02,0xe2,0x26,0xab,0xa3,0x48,0xef,};
    uint8_t d2[32] = {0xc8,0x32,0x9f,0xf4,0x03,0xd6,0xb3,0xd4,0xca,0x58,0xd2,0x0b,0xec,0x9b,0x4c,0xdc,0x1b,0x5a,0x98,0x48,0xb1,0x54,0x6f,0x5a,0xb8,0x12,0x77,0xb9,0xf1,0xfa,0xcc,0x1a,};
    uint8_t expected_k2[32] = {0xec,0x67,0x5d,0x00,0x68,0xec,0xd4,0xd6,0x75,0x1c,0x1a,0xc8,0xa8,0x30,0x07,0xc9,0x5e,0x96,0x77,0xb0,0xa5,0x18,0x76,0x47,0x32,0x41,0x61,0xb4,0x78,0x7b,0x45,0x2d,};
    uint8_t k2[32] = {0};
    ecdh_p256(k2, d2, qx2, qy2);
    assert(is_equal(k2, expected_k2));
    printf("PASS ECDH test 2\n");
    
    uint8_t qx3[32] = {0xed,0x05,0xb7,0xf2,0xcf,0xc3,0xf0,0xef,0x18,0xd5,0x1d,0x19,0x08,0x78,0x3f,0x15,0xb5,0x28,0xcd,0xdc,0x46,0xcf,0xb3,0x19,0x57,0x49,0x55,0xfa,0xb9,0x89,0x39,0xdf,};
    uint8_t qy3[32] = {0xc4,0x6a,0xaf,0xfe,0xe0,0x21,0xe2,0x06,0xfc,0xde,0x70,0xa2,0x54,0xba,0xc8,0x52,0x25,0x75,0xc8,0x06,0x32,0xa3,0x39,0xd7,0x29,0x34,0x00,0x46,0xff,0x94,0x22,0x42,};
    uint8_t d3[32] = {0x2d,0x93,0xb6,0x4d,0x5c,0xc6,0xb6,0x51,0x4d,0xd3,0xc4,0xc9,0xf1,0x9e,0xc4,0x6c,0xb7,0x2f,0x3d,0xf5,0x44,0xb9,0xf4,0xb6,0x3d,0xe0,0xfe,0x9b,0xa7,0x43,0x7c,0x20,};
    uint8_t expected_k3[32] = {0x24,0x00,0x2a,0xbf,0x48,0x3e,0xe7,0x8f,0x4f,0x29,0x9e,0x7d,0x27,0xd4,0x4d,0x09,0xb5,0x17,0xbb,0xa6,0x87,0xd2,0xe3,0xae,0xf6,0x80,0x4b,0x53,0x59,0x12,0x44,0x96,};
    uint8_t k3[32] = {0};
    ecdh_p256(k3, d3, qx3, qy3);
    assert(is_equal(k3, expected_k3));
    printf("PASS ECDH test 3\n");
    
    uint8_t qx4[32] = {0x22,0xb9,0xae,0x0f,0x7b,0xa9,0x74,0x5a,0xb0,0x41,0xe1,0x35,0xc8,0x99,0xa1,0x33,0xa4,0xc1,0x8b,0x3c,0xf5,0xd6,0xa1,0xe6,0x61,0x95,0xe7,0x13,0x28,0x2d,0x19,0x41,};
    uint8_t qy4[32] = {0x28,0x13,0x2a,0x72,0xa0,0x33,0x2e,0xa0,0xff,0xf3,0xeb,0xc8,0x51,0x13,0x28,0x62,0x75,0x2b,0x46,0x5f,0xf3,0x1c,0xb0,0x41,0xe0,0xa7,0x98,0x5e,0xc4,0x8c,0xf9,0x1a,};
    uint8_t d4[32] = {0xbf,0x09,0x5c,0x25,0xd8,0x2f,0x15,0x57,0x47,0xdd,0x08,0xd9,0x5a,0x05,0x38,0x98,0x51,0x3d,0x67,0x39,0x8d,0x71,0xc9,0x95,0xb1,0x50,0x23,0x15,0x38,0x7e,0x13,0x59,};
    uint8_t expected_k4[32] = {0x62,0x8e,0x9a,0x99,0xa6,0x37,0x75,0xf2,0xf7,0x47,0xbf,0x4d,0xe0,0xdd,0xac,0x27,0xce,0x4e,0xcd,0xb8,0x87,0x2a,0xc2,0x12,0xdd,0xe8,0xe8,0x63,0x8d,0x4c,0xd4,0x19,};
    uint8_t k4[32] = {0};
    ecdh_p256(k4, d4, qx4, qy4);
    assert(is_equal(k4, expected_k4));
    printf("PASS ECDH test 4\n");
    
    uint8_t qx5[32] = {0x92,0x27,0x05,0xa3,0x00,0x71,0x5b,0x8e,0xbf,0x74,0x55,0x03,0xb7,0x72,0x31,0x50,0x8b,0xa2,0xfb,0x67,0x58,0x9d,0x64,0xf5,0x38,0xfb,0xf1,0xa0,0x92,0x20,0xe8,0x33,};
    uint8_t qy5[32] = {0x2f,0x4b,0xa1,0x8c,0x6b,0x0d,0xcd,0xd3,0x20,0x0f,0x2c,0x77,0x6c,0x18,0x46,0x2f,0x78,0x8d,0x64,0xbf,0x50,0x55,0x33,0x5e,0x94,0x05,0x0a,0x1e,0x60,0x6b,0xcf,0xf2,};
    uint8_t d5[32] = {0xef,0x32,0xb1,0x40,0x5e,0x25,0x96,0x71,0x0a,0xa9,0x46,0xb3,0x3b,0x9b,0x6c,0xee,0x0f,0xc9,0xe5,0x8c,0xb5,0x79,0x79,0x27,0x61,0xa6,0x10,0x46,0x17,0xe0,0xf8,0xf5,};
    uint8_t expected_k5[32] = {0xd3,0xd3,0x5c,0x52,0x2c,0x2a,0x54,0x03,0x47,0xea,0x9b,0x66,0x5f,0x51,0x9a,0xb1,0xe9,0x4b,0x7e,0x01,0x52,0x5d,0xd6,0x1c,0x93,0xac,0xa4,0xbb,0xd5,0x45,0x4e,0x66,};
    uint8_t k5[32] = {0};
    ecdh_p256(k5, d5, qx5, qy5);
    assert(is_equal(k5, expected_k5));
    printf("PASS ECDH test 5\n");
    

    uint8_t qx6[32] = {0xa7,0xeb,0x6e,0xca,0x3b,0x35,0xb6,0x90,0x4a,0xd3,0x81,0xf9,0x8b,0x4e,0x46,0x90,0x6d,0x68,0x71,0xe5,0x7b,0x14,0x91,0x5c,0x31,0x4e,0x6e,0x91,0x3f,0x0c,0x9e,0x6a,};
    uint8_t qy6[32] = {0x05,0x7f,0x35,0xc5,0xd3,0xc4,0xc6,0x74,0x01,0x9d,0x15,0xad,0xce,0xcb,0xff,0xbd,0xc7,0x3e,0xa7,0xb8,0x75,0x2f,0x60,0xc2,0xbc,0xf2,0xc2,0x39,0xad,0xbe,0xf9,0x40,};
    uint8_t d6[32] = {0x90,0x51,0x8b,0x88,0x82,0x54,0xf3,0x05,0xac,0x76,0xbf,0xb5,0x25,0xbc,0xc6,0xc3,0xd3,0x28,0x3f,0xf6,0x64,0x8b,0x06,0x23,0x9c,0x45,0x03,0xdb,0xf7,0x9a,0x58,0x3b,};
    uint8_t expected_k6[32] = {0x9d,0x67,0x55,0xe0,0xdd,0x8c,0x54,0xbe,0x20,0x68,0x74,0x31,0x49,0xb0,0x5c,0x0c,0xa8,0x60,0x5e,0xc8,0x96,0xc1,0xe7,0x1b,0xd6,0x09,0xdc,0x50,0xaa,0x2d,0x34,0xca,};
    uint8_t k6[32] = {0};
    ecdh_p256(k6, d6, qx6, qy6);
    assert(is_equal(k6, expected_k6));
    printf("PASS ECDH test 6\n");
    

    uint8_t qx7[32] = {0xbb,0x24,0xe2,0x2c,0x41,0x46,0xb0,0x10,0xe2,0xd0,0x1c,0x79,0x75,0x24,0xc8,0x23,0x32,0x71,0xa9,0xb0,0x6f,0xa8,0xd1,0xea,0x73,0x2a,0x5c,0xe5,0xad,0xac,0xc0,0xa9,};
    uint8_t qy7[32] = {0x29,0x42,0x9d,0x4c,0xf0,0x5f,0x2b,0x8c,0x75,0xba,0x03,0xb4,0xc6,0x34,0xd7,0xda,0x3e,0x90,0x41,0xd2,0x53,0xc0,0x67,0x84,0x07,0x3e,0xe9,0x20,0xfa,0x0a,0xde,0xf6,};
    uint8_t d7[32] = {0xc8,0xb9,0x58,0x7f,0x4e,0xd5,0x1d,0xb3,0x22,0xd3,0xc9,0xa4,0xc0,0x26,0xd8,0x61,0x0e,0xc8,0x11,0x1a,0x54,0x1b,0x46,0xb2,0x36,0x74,0xea,0x20,0x9a,0x92,0xbf,0xd8,};
    uint8_t expected_k7[32] = {0x89,0x44,0x3d,0x8b,0xf8,0xc8,0x3a,0x7a,0x6c,0x41,0xf0,0x23,0x79,0xe9,0x7d,0x8c,0x92,0xbe,0x56,0xc7,0x5f,0xe8,0xe4,0xfd,0x1b,0x46,0x6a,0x53,0x52,0x9b,0xaa,0x35,};
    uint8_t k7[32] = {0};
    ecdh_p256(k7, d7, qx7, qy7);
    assert(is_equal(k7, expected_k7));
    printf("PASS ECDH test 7\n");
    
    uint8_t qx8[32] = {0xb9,0x9e,0x6f,0xf3,0x7d,0xdb,0xb7,0x93,0xbd,0x31,0x32,0x2d,0x23,0xb3,0x87,0x54,0xc3,0xaa,0x98,0x95,0x0c,0xac,0xb9,0xf2,0xff,0x55,0x82,0xa9,0x16,0x4f,0xe9,0x94,};
    uint8_t qy8[32] = {0x9a,0xe6,0xdf,0xfe,0x55,0x34,0x6e,0x0b,0x19,0x6e,0x3f,0x5b,0xf1,0x86,0x33,0xf9,0xfb,0xbe,0x6c,0x41,0x94,0x3a,0x09,0xb8,0x90,0xfa,0x9c,0x57,0x43,0x9a,0x04,0xd8,};
    uint8_t d8[32] = {0x4d,0x5b,0xb0,0x2b,0x0b,0xea,0xdb,0x58,0xd4,0x3b,0x7b,0x50,0xf2,0xf1,0x29,0x6a,0x14,0xa5,0x39,0xda,0x8b,0x0d,0xed,0x5d,0xe7,0x2e,0xf3,0x0e,0xba,0x83,0x98,0x0f,};
    uint8_t expected_k8[32] = {0xb4,0x39,0x5f,0xb2,0x90,0xab,0xfb,0x4d,0xe9,0xe9,0x88,0xa8,0xe8,0x03,0x9c,0x69,0xdf,0x00,0xfe,0x63,0x4d,0xf5,0xbf,0xdc,0x75,0xc8,0x9b,0x8a,0x17,0x16,0x5c,0x60,};
    uint8_t k8[32] = {0};
    ecdh_p256(k8, d8, qx8, qy8);
    assert(is_equal(k8, expected_k8));
    printf("PASS ECDH test 8\n");
    
    uint8_t qx9[32] = {0x4f,0x27,0xf5,0xcd,0x4f,0xc9,0x07,0xab,0x64,0x7f,0xd6,0xb5,0x7c,0x12,0x4d,0x00,0x11,0xda,0xf6,0xbb,0x30,0x44,0x54,0xb5,0x60,0x74,0x55,0x4d,0x2a,0xbf,0x99,0xe0,};
    uint8_t qy9[32] = {0xc5,0xc1,0xf8,0x7e,0x8c,0xe9,0xb0,0x4f,0xa6,0x8c,0x68,0x7e,0x51,0x70,0x62,0xeb,0x0e,0x61,0x20,0xe0,0xf4,0x21,0xe2,0xb5,0xed,0x14,0xd7,0x70,0xbe,0x0d,0xc5,0xd9,};
    uint8_t d9[32] = {0x08,0x6c,0xbb,0x37,0x33,0x4a,0x9e,0x33,0xb6,0x53,0x92,0x0c,0x58,0x0d,0xae,0x2c,0xaf,0x3f,0x81,0xbb,0x00,0x75,0xa6,0xf6,0x88,0x69,0x5c,0xb0,0x04,0xdb,0xee,0x2b,};
    uint8_t expected_k9[32] = {0x0b,0x2e,0x09,0x5a,0x83,0x76,0x14,0x26,0x0b,0x98,0x15,0xe7,0xd4,0x73,0xe3,0x95,0x27,0xcc,0x40,0x3c,0xc1,0x2b,0xb6,0x4b,0x85,0x40,0x28,0xb7,0xa1,0x40,0x6e,0xf9,};
    uint8_t k9[32] = {0};
    ecdh_p256(k9, d9, qx9, qy9);
    assert(is_equal(k9, expected_k9));
    printf("PASS ECDH test 9\n");
    
    uint8_t qx10[32] = {0x58,0x16,0xf0,0xcf,0x48,0x82,0x5c,0x64,0x8a,0x77,0x38,0x58,0x82,0xe4,0x07,0xdc,0x12,0xf0,0x6e,0x62,0x96,0x62,0x39,0xc1,0xf3,0x34,0xda,0x6b,0xe5,0x5f,0x5a,0xf7,};
    uint8_t qy10[32] = {0x3c,0x0c,0xfb,0xc3,0x3d,0x8e,0x44,0xbe,0x97,0x7c,0x16,0x19,0xd8,0xa8,0x28,0x2f,0x12,0xf1,0xf3,0x61,0xb0,0x68,0xf5,0x9d,0x05,0xd8,0x72,0x17,0x1b,0xdf,0xbb,0x33,};
    uint8_t d10[32] = {0x48,0xf8,0xe9,0x95,0xc3,0xf6,0xc8,0x72,0xbe,0x1b,0xa1,0xd5,0xe4,0x33,0x93,0x40,0xf1,0xef,0x43,0x89,0x75,0xab,0x6b,0x69,0x41,0x0e,0x61,0x44,0xcf,0x5d,0xc1,0x77,};
    uint8_t expected_k10[32] = {0xa3,0xd3,0xe9,0x28,0x42,0x6e,0xb2,0x81,0x3c,0x27,0xad,0xa4,0x20,0xe4,0x76,0x49,0x55,0x43,0x9e,0x4f,0xa3,0xe8,0xa8,0x02,0xca,0xbd,0xba,0xc4,0x79,0xfa,0x88,0x83,};
    uint8_t k10[32] = {0};
    ecdh_p256(k10, d10, qx10, qy10);
    assert(is_equal(k10, expected_k10));
    printf("PASS ECDH test 10\n");
    
}