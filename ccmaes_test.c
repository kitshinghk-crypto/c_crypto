#include "ccmaes.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h> 
extern bool CCMAES_DEBUG;

void static print_arr(uint8_t* rk, size_t len){
    for(size_t i = 0; i<len; ++i){
        printf("%x ", *(rk+i));
    }
    puts("");
}


int main(void){
    CCMAES_DEBUG = true;
    //40414243 44454647 48494a4b 4c4d4e4f
    uint8_t key[16] = {0x40, 0x41, 0x42, 0x43, 
                        0x44, 0x45, 0x46, 0x47, 
                        0x48, 0x49, 0x4a, 0x4b, 
                        0x4c, 0x4d, 0x4e, 0x4f};
    //20212223
    uint8_t p[4] = {0x20, 0x21, 0x22, 0x23};
    //10111213 141516
    uint8_t n[7] = {0x10, 0x11, 0x12, 0x13, 
                    0x14, 0x15, 0x16};
    //00010203 04050607
    uint8_t a[8] = {0x00, 0x01, 0x02, 0x03, 
                    0x04, 0x05, 0x06, 0x07}; 
    
    uint8_t* cipher = malloc(8 * sizeof(uint8_t*));
    printf("CCM AES encrypt:\n");
    ccm_aes_encrypt(cipher, p, key, n, a, 7, 8, 4*sizeof(uint8_t), 4);
    printf("cipher:\n");
    print_arr(cipher, 8);

    puts("");
    //71 62 1 5b 4d ac 25 5d
    uint8_t cipher3[8] = {0x71, 0x62, 0x01, 0x5b, 0x4d, 0xac, 0x25, 0x5d};
    uint8_t* p3 = malloc(4 * sizeof(uint8_t*));
    ccm_aes_decrypt(p3, cipher3, key, n, a, 7, 8, 8, 4);
    printf("plaintext:\n");
    print_arr(p3, 8);

    puts("");
    //20212223 24252627 28292a2b 2c2d2e2f
    uint8_t p2[16] = {0x20, 0x21, 0x22, 0x23,
                    0x24, 0x25, 0x26, 0x27, 
                    0x28, 0x29, 0x2a, 0x2b, 
                    0x2c, 0x2d, 0x2e, 0x2f};
    //10111213 14151617
    uint8_t n2[8] = {0x10, 0x11, 0x12, 0x13, 
                    0x14, 0x15, 0x16, 0x17};
    //00010203 04050607 08090a0b 0c0d0e0f
    uint8_t a2[16] = {0x00, 0x01, 0x02, 0x03, 
                    0x04, 0x05, 0x06, 0x07,
                    0x08, 0x09, 0x0a, 0x0b,
                    0x0c, 0x0d, 0x0e, 0x0f}; 
    uint8_t* cipher2 = malloc(16 * sizeof(uint8_t*));
    printf("CCM AES encrypt:\n");
    ccm_aes_encrypt(cipher2, p2, key, n2, a2, 8, 16, 16*sizeof(uint8_t), 6);
    printf("cipher:\n");
    print_arr(cipher2, 22);


    puts("");
    //d2 a1 f0 e0 51 ea 5f 62 8 1a 77 92 7 3d 59 3d 1f c6 4f bf ac cd
    uint8_t cipher4[22] = {0xd2, 0xa1, 0xf0, 0xe0, 0x51, 0xea, 0x5f, 0x62,
                        0x08, 0x1a, 0x77, 0x92, 0x07, 0x3d, 0x59, 0x3d,
                        0x1f, 0xc6, 0x4f, 0xbf, 0xac, 0xcd};
    uint8_t* p4 = malloc(16 * sizeof(uint8_t*));
    ccm_aes_decrypt(p4, cipher4, key, n2, a2, 8, 16, 22, 6);
    printf("plaintext:\n");
    print_arr(p4, 8);
}