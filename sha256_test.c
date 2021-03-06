#include "sha256.h"
#include <stdio.h>

extern bool SHA256_DEBUG;

void print_arr(uint8_t* rk, unsigned int len){
    for(unsigned int i =0; i<len; i++){
        printf("%x ", rk[i]);
    }
    puts("");
}

void print_arr32(uint32_t* rk, unsigned int len){
    for(int i =0; i<len; i++){
        printf("%08x ", rk[i]);
    }
    puts("");
}

void testsha256(uint8_t* m, size_t len){
    SHA256_DEBUG = true;
    uint32_t hash[8]={0};
    sha256_hash(hash, m, len);
    printf("SHA256 hash:\n");
    printf("m:\n");
    print_arr(m, len/8);
    printf("hash:\n");
    print_arr32(hash, 8);
}

int main(){
    uint8_t m[12]={0x68, 0x65, 0x6C, 0x6C ,0x6F ,0x20 ,0x77 ,0x6F, 0x72, 0x6C, 0x64};
    testsha256(m, 88);

    //expect: f757fe6ec7239f7e9f6accade3990a15e74e435a932c48ecccfa70a66c3fdb9d
    uint8_t m2[64] = {0xf6, 0xbb, 0x5d, 0x59, 0xb0, 0xfa, 0x9d, 0xe0, 
                0x82, 0x8b, 0x11, 0x53, 0x03, 0xbf, 0x94, 0xaa, 
                0x98, 0x73, 0x61, 0xcc, 0xdd, 0xe4, 0x8d, 0x02, 
                0x46, 0xc5, 0xd5, 0xab, 0x06, 0x8f, 0x9a, 0x32, 
                0x2f, 0x19, 0x2a, 0x3e, 0x1b, 0x68, 0x41, 0x28, 
                0x0c, 0xc8, 0xd0, 0xb2, 0x0f, 0x1b, 0xfc, 0xf6, 
                0x26, 0x72, 0x6a, 0x9c, 0xa5, 0xda, 0xba, 0x50, 
                0xdd, 0x79, 0x51, 0x73, 0xf8, 0xd9, 0x5c, 0x11};
    testsha256(m2, 512);

    //expect: 5ca6a57e2ecc12065487f1d4d75aa27b16df44cb41a64d2b6d509321e2d14b5b
    uint8_t m3[61] = {0x58, 0xf3, 0x1e, 0x57, 0x70, 0x07, 0x0a, 0x5d, 0x40, 
                0x31, 0xfb, 0x79, 0x5d, 0xc2, 0xd2, 0x98, 0x56, 0x1d, 
                0x35, 0x59, 0x96, 0x0d, 0x8f, 0x09, 0x8c, 0x5c, 0x57, 
                0xd2, 0x09, 0xa1, 0x96, 0x2f, 0xe9, 0x59, 0xa3, 0xa3, 
                0xf5, 0x39, 0x31, 0xab, 0x08, 0xc3, 0x8c, 0xf1, 0x71, 
                0x18, 0x20, 0xc2, 0x99, 0x0d, 0x3b, 0xc2, 0xe6, 0x80, 
                0x14, 0xdf, 0xde, 0xed, 0x0b, 0x83, 0x90};
    testsha256(m3, 485);    
}