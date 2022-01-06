#include "aes.h"
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
extern bool AES_DEBUG;

int is_equal(uint8_t* expected, uint8_t* actual){
    for(size_t i =0; i<16;i++){
        if(expected[i]!=actual[i]){
            return 0;
        }
    }
    return 1;
}

int main(void){
    uint8_t key[16]={0x54, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6D, 0x79, 0x20, 0x4B, 0x75, 0x6E, 0x67, 0x20, 0x46, 0x75};
    uint8_t m[16] = {0x54, 0x77, 0x6F, 0x20, 0x4F, 0x6E, 0x65 ,0x20, 0x4E, 0x69 ,0x6E, 0x65, 0x20, 0x54, 0x77, 0x6F};
    uint8_t cipher[16] = {0x29, 0xC3, 0x50, 0x5F, 0x57, 0x14, 0x20, 0xF6, 0x40, 0x22, 0x99, 0xB3, 0x1A, 0x02, 0xD7, 0x3A};
    uint8_t actual_cipher[16] = {0};
    uint8_t actual_plaintext[16] = {0};
    aes_encrypt(actual_cipher, key, m);
    assert(is_equal(cipher, actual_cipher));
    aes_decrypt(actual_plaintext, key, cipher);
    assert(is_equal(m, actual_plaintext));


    uint8_t key2[16]={0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    uint8_t m2[16]={0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    uint8_t cipher2[16] = {0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30, 0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a};
    uint8_t actual_cipher2[16] = {0};
    uint8_t actual_plaintext2[16] = {0};
    aes_encrypt(actual_cipher2, key2, m2);
    assert(is_equal(cipher2, actual_cipher2));
    aes_decrypt(actual_plaintext2, key2, cipher2);
    assert(is_equal(m2, actual_plaintext2));
}