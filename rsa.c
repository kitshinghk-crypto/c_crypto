#include "rsa.h"
#include "arithmetic.h"
#include "sha256_8u.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#define WORD_LENGTH 256

const static uint8_t SHA256_SIG[19] ={0x30,0x31,0x30,0x0d,0x06,0x09,0x60,0x86,0x48,0x01,0x65,0x03,0x04,0x02,0x01,0x05,0x00,0x04,0x20};

void static reverse_msg(uint8_t * r_msg, const uint8_t* msg, size_t msg_byte_len){
    for(size_t i=0; i<msg_byte_len; i++){
        r_msg[msg_byte_len-i-1] = msg[i];
    }
}

void pkcs1_15_sha256_sig_gen(uint8_t* s, const uint8_t* m, const uint8_t* n, const uint8_t* d, size_t mlen){
    size_t byte_len = mlen/8 + (mlen%8 > 0);
    uint8_t * r_msg = malloc(sizeof(uint8_t)*byte_len);
    if (r_msg ==0) return;
    uint8_t hash[32]={0};
    uint16_t h_16[32] = {0};
    uint16_t em[WORD_LENGTH] = {0};
    uint16_t n16[WORD_LENGTH] = {0};
    uint16_t d16[WORD_LENGTH] = {0};
    for(int i=0; i<WORD_LENGTH;i++){
        n16[i]=n[i]; d16[i] = d[i];
    }

    reverse_msg(r_msg, m, byte_len);
    sha256_8u_hash(hash, r_msg, mlen);
    for(size_t i=0; i< 32; i++){
        h_16[i] = hash[i];
    }
    em[WORD_LENGTH-1] = 0x00;
    em[WORD_LENGTH-2] = 0x01;
    for(int i=1; i<=202; i++){
        em[WORD_LENGTH-i-2] = 0xff;
    }
    em[WORD_LENGTH-2-202-1] = 0x00;
    for(int i=0; i<19; i++){
        em[WORD_LENGTH-2-202-1-(i+1)]=SHA256_SIG[i];
    }
    for(int i=0; i<32; i++){
        em[WORD_LENGTH-2-202-1-19-i-1]=h_16[i];
    }
    mont_exp(em,d16,n16,WORD_LENGTH*8,WORD_LENGTH);
    for(int i=0; i<WORD_LENGTH; i++){
        s[i]=em[i]&0xff;
    }
    free(r_msg); r_msg=0;
}

int pkcs1_15_sha256_sig_ver(const uint8_t* s, const uint8_t* m, const uint8_t* n, const uint8_t* e, size_t mlen){
    size_t byte_len = mlen/8 + (mlen%8 > 0);
    uint8_t * r_msg = malloc(sizeof(uint8_t)*byte_len);
    if (r_msg == 0) return 0;
    uint16_t n16[WORD_LENGTH] = {0};
    uint16_t e16[WORD_LENGTH] = {0};
    uint16_t s16[WORD_LENGTH] = {0};
    uint8_t hash[32]={0};
    uint16_t h_16[32] = {0};
    for(int i=0; i<WORD_LENGTH;i++){
        n16[i]=n[i]; e16[i] = e[i]; s16[i] = s[i];
    }
    mont_exp(s16,e16,n16,WORD_LENGTH*8,WORD_LENGTH);
    if (s16[WORD_LENGTH-1] != 0x00){
        return 0;
    }
    if (s16[WORD_LENGTH-2] != 0x01){
        return 0;
    }

    for(int i=1; i<=202; i++){
        if(s16[WORD_LENGTH-i-2] != 0xff){
            return 0;
        }
    }
    if(s16[WORD_LENGTH-2-202-1] != 0x00){
        return 0;
    }
    
    if (s16[WORD_LENGTH-2-202-1] != 0x00){
        return 0;
    }

    for(int i=0; i<19; i++){
        if(s16[WORD_LENGTH-2-202-1-(i+1)]!=SHA256_SIG[i]){
            return 0;
        }
    }
    

    reverse_msg(r_msg, m, byte_len);
    sha256_8u_hash(hash, r_msg, mlen);
    for(size_t i=0; i< 32; i++){
        h_16[i] = hash[i];
    }

    for(int i=0; i<32; i++){
        if(s16[WORD_LENGTH-2-202-1-19-i-1]!=h_16[i]){
            return 0;
        }
    }
    return 1;
}