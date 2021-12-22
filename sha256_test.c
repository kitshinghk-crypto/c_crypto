#include "sha256.h"
#include <stdio.h>

extern bool SHA256_DEBUG;

void print_arr32(uint32_t* rk, unsigned int len){
    for(int i =0; i<len; i++){
        printf("%08x ", rk[i]);
    }
    puts("");
}

void testsha256(){
    SHA256_DEBUG = true;
    //68 65 6C 6C 6F 20 77 6F 72 6C 64
    uint8_t m[12]={0x68, 0x65, 0x6C, 0x6C ,0x6F ,0x20 ,0x77 ,0x6F, 0x72, 0x6C, 0x64, 0x00};
    uint32_t hash[8]={0};
    sha256_hash(hash, m);
    printf("SHA256 hash:\n");
    print_arr32(hash, 8);
}

int main(){
    testsha256();
}