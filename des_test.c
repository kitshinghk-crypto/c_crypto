#include "des.h"
#include <stdio.h>
#include <assert.h>

extern bool DES_DEBUG;

void testdesencrypt(uint8_t* key, uint8_t* pt){
    DES_DEBUG = false;
    uint8_t cipher[8];
    printf("DES encrypt:\n");
    puts("plaintext: ");
    for(int i=0; i<8; ++i){
        printf("%x ", pt[i]);
    }
    puts("");
    puts("key: ");
    for(int i=0; i<8; ++i){
        printf("%x ", key[i]);
    }
    puts("");
    des_encrypt(cipher, key, pt);
    puts("chipher: ");
    for(int i=0; i<8; ++i){
        printf("%x ", cipher[i]);
    }
    puts("");
}


void testdesdecrypt(uint8_t* key, uint8_t* cipher){
    DES_DEBUG = false;
    uint8_t pt[8];
    printf("DES decrypt:\n");
    puts("chipher: ");
    for(int i=0; i<8; ++i){
        printf("%x ", cipher[i]);
    }
    puts("");
    puts("key: ");
    for(int i=0; i<8; ++i){
        printf("%x ", key[i]);
    }
    puts("");
    des_decrypt(pt, key, cipher);
    puts("plaintext: ");
    for(int i=0; i<8; ++i){
        printf("%x ", pt[i]);
    }
    puts("");
}


int is_equal(uint8_t* expected, uint8_t* actual){
    for(size_t i =0; i<8;i++){
        if(expected[i]!=actual[i]){
            return 0;
        }
    }
    return 1;
}

int main(void){
    uint8_t pt[8]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};
    uint8_t key[8]={0x13,0x34,0x57,0x79,0x9B,0xBC,0xDF,0xF1};
    uint8_t cipher[8]={0x85,0xe8,0x13,0x54,0x0f,0x0a,0xb4,0x05};
    uint8_t actual_cipher[8]={0};
    uint8_t actual_pt[8]={0};
    des_encrypt(actual_cipher, key, pt);
    assert(is_equal(cipher, actual_cipher));
    des_decrypt(actual_pt, key, cipher);
    assert(is_equal(pt, actual_pt));
}
