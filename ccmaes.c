#include "ccmaes.h"
#include "aes.h"
#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>
#define D if(CCMAES_DEBUG)

bool CCMAES_DEBUG = false;

void static print_arr(uint8_t* rk, size_t len){
    for(size_t i = 0; i<len; ++i){
        printf("%x ", rk[i]);
    }
    puts("");
}

// x = x ^ y
int xor(uint8_t* x, uint8_t* y, size_t len){
    for(size_t i=0; i< len; i++){
        x[i] = x[i] ^ y[i];
    }
    return 1;
}

int ccm_aes_encrypt(uint8_t* cipher, const uint8_t* plaintext, const uint8_t* key, const uint8_t* nonce,
                            const uint8_t* a, uint8_t nlen, size_t alen, size_t plen, uint8_t tlen){
    D{printf("plen=%zu\n", plen);}
    uint8_t b[16]={0};
    size_t q=plen;
    uint8_t qlen = 15-nlen;
    uint8_t y[16]={0};
    //construct b0
    b[0] |= (alen>0)? 0x40:0x00;
    b[0] |= (((tlen-2)>>2)<<4) & 0x30;
    b[0] |= ((((tlen-2)>>1)&0x01)<<3) & 0x08;
    b[0] |= (qlen-1) & 0x07;
    //Copy nonce. Start from b[1]
    for(uint8_t i=0; i<nlen; i++){
        b[i+1] = nonce[i];
    }
    //Update the size of the plaintext at the end of b0
    for(uint8_t m = 0; m < qlen; m++, q >>= 8){
        b[15 - m] = q & 0xFF;
    }
    //aes(b) -> y0
    D{printf("b:\n");}
    D{print_arr(b,16);}
    aes_encrypt(y,key,b);
    //A: associated data
    uint8_t curr_b_ind = 0u;
    if(alen>0){
        if(alen<65280){
            b[curr_b_ind++] = (alen>>8) & 0xff;
            b[curr_b_ind++] = alen & 0xff;
        }else if (alen>=65280 && alen<4294967296){
            b[curr_b_ind++] = 0xff;
            b[curr_b_ind++] = 0xfe;
            for(uint8_t i = 24; i>=0; i-=8){
                b[curr_b_ind++] = (alen >> i) & 0xff; 
            }
        }else{
            b[curr_b_ind++] = 0xff;
            b[curr_b_ind++] = 0xff;
            for(uint8_t i = 56; i>=0; i-=8){
                b[curr_b_ind++] = (alen >> i) & 0xff;
            }
        }
        //Copy A to B
        for(size_t i=0; i < alen; i++){
            b[curr_b_ind++] = a[i];
            if(curr_b_ind>=16){
                D{printf("b:\n");}
                D{print_arr(b,16);}
                xor(b, y, 16);
                aes_encrypt(y,key,b);
                curr_b_ind = 0;
            }
        }
        //Pad B with 0x00
        if(curr_b_ind>0 && curr_b_ind<16){
            while(curr_b_ind<16){
                b[curr_b_ind++] = 0x00;
            }
            D{printf("b:\n");}
            D{print_arr(b,16);}
            xor(b, y, 16);
            aes_encrypt(y,key,b);
            curr_b_ind = 0;
        }
    }

    //Copy P to B
    for(size_t i=0; i < plen; i++){
        b[curr_b_ind++] = plaintext[i];
        if(curr_b_ind>=16){
            D{printf("b:\n");}
            D{print_arr(b,16);}
            xor(b, y, 16);
            aes_encrypt(y,key,b);
            curr_b_ind = 0;
        }
    }
    //Pad B with 0x00
    if(curr_b_ind>0 && curr_b_ind<16){
        while(curr_b_ind<16){
            b[curr_b_ind++] = 0x00;
        }
        D{printf("b:\n");}
        D{print_arr(b,16);}
        xor(b, y, 16);
        aes_encrypt(y,key,b);
    }

    D{printf("T:\n");}
    D{print_arr(y,16);}
    // Compute CTR
    size_t m = ((plen/16) + (plen%16>0));
    uint8_t** ctr = malloc((m+1) * sizeof(uint8_t*));
    for (size_t i = 0; i < m+1; i++) {
        ctr[i] = malloc(16 * sizeof(uint8_t));
    }
    for(size_t i = 0; i < m+1; i++){
        ctr[i][0] = (qlen-1) & 0x07;
        for(size_t ni =0; ni < nlen; ni++){
            ctr[i][ni+1] = nonce[ni];
        }
        size_t ci = 15;
        if(i>0){
            ctr[i][15] = ctr[i-1][15] + 1;
            while(ctr[i][ci] == 0 && ci >= 16-qlen ){
                ctr[i][ci-1] ++;
                ci--;
            }
        }
        D{printf("CTR%zu:\n",i);}
        D{print_arr(ctr[i],16);}
    }
    // Compute S
    uint8_t** s = malloc((m+1) * sizeof(uint8_t*));
    for (size_t i = 0; i < m+1; i++) {
        s[i] = malloc(16 * sizeof(uint8_t));
    }
    for(size_t i=0; i< m+1; ++i){
        aes_encrypt(s[i], key, ctr[i]);
        D{printf("S%zu:\n",i);}
        D{print_arr(s[i],16);}
    }
    // Compute C
    for(size_t i=0; i<plen; ++i){
        *(cipher+i) = plaintext[i] ^ s[(i/16)+1][i%16];
    }
    for(size_t i=(plen); i<(plen + tlen); ++i){
        *(cipher+i) = y[i-(plen)] ^ s[0][i-(plen)];
    }
    D{printf("C:\n");}
    D{print_arr(cipher,(plen + tlen));}
    return 1;
}

int ccm_aes_decrypt(uint8_t* plaintext, const uint8_t* cipher, const uint8_t* key, const uint8_t* nonce,
                            const uint8_t* a, uint8_t nlen, size_t alen, size_t clen, uint8_t tlen){
    uint8_t qlen = 15-nlen;
    //Compute CTR
    size_t m = (((clen-tlen)/16) + ((clen-tlen)%16>0));
    uint8_t** ctr = malloc((m+1) * sizeof(uint8_t*));
    for (size_t i = 0; i < m+1; i++) {
        ctr[i] = malloc(16 * sizeof(uint8_t));
    }
    for(size_t i = 0; i < m+1; i++){
        ctr[i][0] = (qlen-1) & 0x07;
        for(size_t ni =0; ni < nlen; ni++){
            ctr[i][ni+1] = nonce[ni];
        }
        size_t ci = 15;
        if(i>0){
            ctr[i][15] = ctr[i-1][15] + 1;
            while(ctr[i][ci] == 0 && ci >= 16-qlen ){
                ctr[i][ci-1] ++;
                ci--;
            }
        }
        D{printf("CTR%zu:\n",i);}
        D{print_arr(ctr[i],16);}
    }
    // Compute S
    uint8_t** s = malloc((m+1) * sizeof(uint8_t*));
    for (size_t i = 0; i < m+1; i++) {
        s[i] = malloc(16 * sizeof(uint8_t));
    }
    for(size_t i=0; i< m+1; ++i){
        aes_encrypt(s[i], key, ctr[i]);
        D{printf("S%zu:\n",i);}
        D{print_arr(s[i],16);}
    }
    // Compute P
    for(size_t i=0; i<clen-tlen; i++){
        plaintext[i] = cipher[i] ^ s[(i/16)+1][i%16];
    }
    D{printf("P:\n");}
    D{print_arr(plaintext,(clen-tlen));}
    uint8_t* t = malloc(tlen * sizeof(uint8_t));
    for(size_t i=clen-tlen; i<clen; i++){
        *(t+(i-(clen-tlen))) = cipher[i] ^ s[0][(i-(clen-tlen))];
    }
    D{printf("T:\n");}
    D{print_arr(t,tlen);}

    //Compute b
    uint8_t b[16]={0};
    size_t q=clen-tlen;
    uint8_t y[16]={0};
    //construct b0
    b[0] |= (alen>0)? 0x40:0x00;
    b[0] |= (((tlen-2)>>2)<<4) & 0x30;
    b[0] |= ((((tlen-2)>>1)&0x01)<<3) & 0x08;
    b[0] |= (qlen-1) & 0x07;
    //Copy nonce. Start from b[1]
    for(uint8_t i=0; i<nlen; i++){
        b[i+1] = nonce[i];
    }
    //Update the size of the plaintext at the end of b0
    for(uint8_t m = 0; m < qlen; m++, q >>= 8){
        b[15 - m] = q & 0xFF;
    }
    //aes(b) -> y0
    D{printf("b:\n");}
    D{print_arr(b,16);}
    aes_encrypt(y,key,b);
    //A: associated data
    uint8_t curr_b_ind = 0u;
    if(alen>0){
        if(alen<65280){
            b[curr_b_ind++] = (alen>>8) & 0xff;
            b[curr_b_ind++] = alen & 0xff;
        }else if (alen>=65280 && alen<4294967296){
            b[curr_b_ind++] = 0xff;
            b[curr_b_ind++] = 0xfe;
            for(uint8_t i = 24; i>=0; i-=8){
                b[curr_b_ind++] = (alen >> i) & 0xff; 
            }
        }else{
            b[curr_b_ind++] = 0xff;
            b[curr_b_ind++] = 0xff;
            for(uint8_t i = 56; i>=0; i-=8){
                b[curr_b_ind++] = (alen >> i) & 0xff;
            }
        }
        //Copy A to B
        for(size_t i=0; i < alen; i++){
            b[curr_b_ind++] = a[i];
            if(curr_b_ind>=16){
                D{printf("b:\n");}
                D{print_arr(b,16);}
                xor(b, y, 16);
                aes_encrypt(y,key,b);
                curr_b_ind = 0;
            }
        }
        //Pad B with 0x00
        if(curr_b_ind>0 && curr_b_ind<16){
            while(curr_b_ind<16){
                b[curr_b_ind++] = 0x00;
            }
            D{printf("b:\n");}
            D{print_arr(b,16);}
            xor(b, y, 16);
            aes_encrypt(y,key,b);
            curr_b_ind = 0;
        }
    }

    //Copy P to B
    for(size_t i=0; i < clen-tlen; i++){
        b[curr_b_ind++] = plaintext[i];
        if(curr_b_ind>=16){
            D{printf("b:\n");}
            D{print_arr(b,16);}
            xor(b, y, 16);
            aes_encrypt(y,key,b);
            curr_b_ind = 0;
        }
    }
    //Pad B with 0x00
    if(curr_b_ind>0 && curr_b_ind<16){
        while(curr_b_ind<16){
            b[curr_b_ind++] = 0x00;
        }
        D{printf("b:\n");}
        D{print_arr(b,16);}
        xor(b, y, 16);
        aes_encrypt(y,key,b);
    }
    D{printf("Y:\n");}
    D{print_arr(y,16);}
    //validate T
    for(size_t i=0; i<tlen; i++){
        if(t[i]!=y[i]){
            D{printf("T invalid\n");}
            return 0;
        }
    }
    D{printf("T valid\n");}

    return 1;
}