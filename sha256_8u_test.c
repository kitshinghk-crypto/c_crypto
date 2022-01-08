#include "sha256_8u.h"
#include <stdio.h>
#include <assert.h>

extern bool SHA256_8U_DEBUG;

void print_arr(uint8_t* rk, unsigned int len){
    for(unsigned int i =0; i<len; i++){
        printf("%02x ", rk[i]);
    }
    puts("");
}

void testsha256(uint8_t* m, size_t len){
    SHA256_8U_DEBUG = true;
    uint8_t hash[32]={0};
    sha256_8u_hash(hash, m, len);
    printf("SHA256 hash:\n");
    printf("m:\n");
    print_arr(m, len/8);
    printf("hash:\n");
    print_arr(hash, 32);
}

int is_equal(uint8_t* expected, uint8_t* actual){
    for(size_t i =0; i<32;i++){
        if(expected[i]!=actual[i]){
            return 0;
        }
    }
    return 1;
}

int main(){
    //uint8_t m[12]={0x68, 0x65, 0x6C, 0x6C ,0x6F ,0x20 ,0x77 ,0x6F, 0x72, 0x6C, 0x64};
    //testsha256(m, 88);

    //expect: f757fe6ec7239f7e9f6accade3990a15e74e435a932c48ecccfa70a66c3fdb9d
    uint8_t m2[64] = {0xf6, 0xbb, 0x5d, 0x59, 0xb0, 0xfa, 0x9d, 0xe0, 
                0x82, 0x8b, 0x11, 0x53, 0x03, 0xbf, 0x94, 0xaa, 
                0x98, 0x73, 0x61, 0xcc, 0xdd, 0xe4, 0x8d, 0x02, 
                0x46, 0xc5, 0xd5, 0xab, 0x06, 0x8f, 0x9a, 0x32, 
                0x2f, 0x19, 0x2a, 0x3e, 0x1b, 0x68, 0x41, 0x28, 
                0x0c, 0xc8, 0xd0, 0xb2, 0x0f, 0x1b, 0xfc, 0xf6, 
                0x26, 0x72, 0x6a, 0x9c, 0xa5, 0xda, 0xba, 0x50, 
                0xdd, 0x79, 0x51, 0x73, 0xf8, 0xd9, 0x5c, 0x11};
    uint8_t expected_hash1[32] = {0xf7, 0x57, 0xfe, 0x6e, 0xc7, 0x23, 0x9f, 0x7e, 0x9f, 0x6a, 0xcc, 0xad, 0xe3, 0x99, 0x0a, 0x15, 0xe7, 0x4e, 0x43, 0x5a, 0x93, 0x2c, 0x48, 0xec, 0xcc, 0xfa, 0x70, 0xa6, 0x6c, 0x3f, 0xdb, 0x9d};
    uint8_t hash1[32]={0};
    sha256_8u_hash(hash1, m2, 512);
    printf("hash:\n");
    print_arr(hash1, 32);
    assert(is_equal(hash1, expected_hash1));
    printf("PASS SHA256 hash test 1\n");

    //expect: 5ca6a57e2ecc12065487f1d4d75aa27b16df44cb41a64d2b6d509321e2d14b5b
    //        5ca6a57e2ecc12065487f1d4d75aa27b16df44cb41a64d2b6d509321e2d14b5b
    uint8_t m3[61] = {0x58, 0xf3, 0x1e, 0x57, 0x70, 0x07, 0x0a, 0x5d, 0x40, 
                0x31, 0xfb, 0x79, 0x5d, 0xc2, 0xd2, 0x98, 0x56, 0x1d, 
                0x35, 0x59, 0x96, 0x0d, 0x8f, 0x09, 0x8c, 0x5c, 0x57, 
                0xd2, 0x09, 0xa1, 0x96, 0x2f, 0xe9, 0x59, 0xa3, 0xa3, 
                0xf5, 0x39, 0x31, 0xab, 0x08, 0xc3, 0x8c, 0xf1, 0x71, 
                0x18, 0x20, 0xc2, 0x99, 0x0d, 0x3b, 0xc2, 0xe6, 0x80, 
                0x14, 0xdf, 0xde, 0xed, 0x0b, 0x83, 0x90};
    uint8_t expected_hash2[32]={0x5c, 0xa6, 0xa5, 0x7e, 0x2e, 0xcc, 0x12, 0x06, 0x54, 0x87, 0xf1, 0xd4, 0xd7, 0x5a, 0xa2, 0x7b, 0x16, 0xdf, 0x44, 0xcb, 0x41, 0xa6, 0x4d, 0x2b, 0x6d, 0x50, 0x93, 0x21, 0xe2, 0xd1, 0x4b, 0x5b};
    uint8_t hash2[32]={0};
    sha256_8u_hash(hash2, m3, 485);
    printf("hash:\n");
    print_arr(hash2, 32);
    assert(is_equal(hash2, expected_hash2)); 
    printf("PASS SHA256 hash test 2\n");
}