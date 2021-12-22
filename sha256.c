#include "sha256.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


bool SHA256_DEBUG = false;

const static uint32_t ih[8] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

const static uint32_t k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

void print_arr(unsigned char* rk, unsigned int len){
    for(int i =0; i<len; i++){
        printf("%x ", rk[i]);
    }
    puts("");
}

void static print_arr32(uint32_t* rk, unsigned int len){
    for(int i =0; i<len; i++){
        printf("%08x ", rk[i]);
    }
    puts("");
}

void printBits(size_t const size, uint32_t const * const ptr){
    uint32_t const *b = ptr;
    uint8_t byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 31; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
    }
    puts("");
}

int pad(uint8_t * m, size_t pm_len, size_t start_pad_ind){
    for(size_t i = start_pad_ind; i<pm_len; ++i){
        *(m+i) = ((1U<<7)*(i==start_pad_ind))|0U;
    }
    return 1;
}

int pad_m_len(uint8_t * m, size_t pm_len, size_t m_len){
    for(uint8_t i =0;i<8;i++){
        *(m+(pm_len-i-1)) = m_len>>(i*8) & 0xff;
    }
    return 1;
}

int init_h(uint32_t* h){
    for(uint8_t i =0;i<8;++i){
        h[i] = ih[i];
    }
    return 1;
}

uint32_t rotr(uint32_t x, uint32_t shift){
    return (x>>shift)| (x<<(32-shift));
 }

uint32_t func_s0(uint32_t x){
    return rotr(x,7) ^ rotr(x,18) ^ (x>>3); 
}

uint32_t func_s1(uint32_t x){
    return rotr(x,17) ^ rotr(x,19) ^ (x>>10);
}

uint32_t func_e0(uint32_t x){
    return rotr(x, 2)^rotr(x, 13)^rotr(x,22);
}

uint32_t func_e1(uint32_t x){
    return rotr(x, 6)^rotr(x, 11)^rotr(x,25);   
}

uint32_t func_ch(uint32_t x, uint32_t y, uint32_t z){
    return (((x) & (y)) ^ (~(x) & (z)));
}

uint32_t func_maj(uint32_t x, uint32_t y, uint32_t z){
    return (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)));
}

int init_w(uint32_t* w, uint8_t* m){
    for(uint8_t i=0; i<64; ++i){
        w[i/4]|=m[i]<<(8*(3-(i%4)));
    }
    for(uint8_t i=16;i<64;++i){
        w[i] = func_s1(w[i-2]) + w[i-7] + func_s0(w[i-15]) + w[i-16];
    }
    return 1;
}

int sha256_hash(uint32_t* out, uint8_t* m){
    uint32_t a=ih[0], b=ih[1], c=ih[2], d=ih[3], e=ih[4], f=ih[5], g=ih[6], h=ih[7];
    size_t len = strlen((char*)m);
    printf(" Message : %s\n", (char*)m);
    printf(" Message length: %zu\n", len);

    size_t m_bit_len = len * sizeof(uint8_t) * 8;
    printf(" Message bit length: %zu\n", m_bit_len);
    size_t byte_pad_len = ((448 + 512)-(m_bit_len+1)%512)%512;
    printf(" Message pad bit length: %zu\n", byte_pad_len+1);
    size_t padded_m_len = len + (byte_pad_len+1)/8+8;
    printf(" Padded message len: %zu\n", padded_m_len);
    uint8_t * padded_m = malloc(sizeof(uint8_t)* padded_m_len);
    for(uint8_t i =0; i< len; ++i){
        *(padded_m+i) = m[i];
    }
    pad(padded_m, padded_m_len, len);
    printf(" Padded message: \n");
    print_arr(padded_m, padded_m_len);
    pad_m_len(padded_m, padded_m_len, m_bit_len);
    printf(" Padded message with message size: \n");
    print_arr(padded_m, padded_m_len);

    uint32_t hash[8] = {0};
    init_h(hash);
    for (uint32_t round =0; round<padded_m_len/64; ++round){
        printf("Round=%u\n", round);
        uint32_t w[64] = {0};
        init_w(w, padded_m+(round*64));
        printf(" init w: \n");    
        print_arr32(w, 64);
        a=hash[0];b=hash[1];c=hash[2];d=hash[3];e=hash[4];f=hash[5];g=hash[6];h=hash[7];
        uint32_t t1=0, t2=0;
        for(uint8_t i=0; i<64; ++i){
            t1 = h + func_e1(e) + func_ch(e,f,g) + k[i] + w[i];
            t2 = func_e0(a) + func_maj(a,b,c);
            h=g; g=f; f=e; e=d+t1; d=c; c=b; b=a; a=t1+t2;
        }
        printf(" a: \n");print_arr32(&a, 1);
        printf(" b: \n");print_arr32(&b, 1);
        printf(" c: \n");print_arr32(&c, 1);
        printf(" d: \n");print_arr32(&d, 1);
        printf(" e: \n");print_arr32(&e, 1);
        printf(" f: \n");print_arr32(&f, 1);
        printf(" g: \n");print_arr32(&g, 1);
        printf(" h: \n");print_arr32(&h, 1);

        hash[0] +=a; hash[1] +=b; hash[2] +=c; hash[3] +=d; hash[4] +=e; hash[5] +=f; hash[6] +=g; hash[7] +=h;
        printf(" hash0: \n");print_arr32(hash, 1);
        printf(" hash1: \n");print_arr32(hash+1, 1);
        printf(" hash2: \n");print_arr32(hash+2, 1);
        printf(" hash3: \n");print_arr32(hash+3, 1);
        printf(" hash4: \n");print_arr32(hash+4, 1);
        printf(" hash5: \n");print_arr32(hash+5, 1);
        printf(" hash6: \n");print_arr32(hash+6, 1);
        printf(" hash7: \n");print_arr32(hash+7, 1);
    }
    for(uint8_t i=0;i<8;++i){
        out[i] = hash[i];
    }
    return 1;
}