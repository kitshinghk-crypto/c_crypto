#include "hash_drbg.h"
#include "sha256_8u.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#define D if(HASH_DRBG_DEBUG)
#define OUTLEN 256
#define SEEDLEN 440

void print_hex(uint8_t* a, size_t len){
    for(int i = 0; i<len; i++){
        printf("%02x ", a[i]);
    }
    printf("\n");
}

void static print_state(struct hashdrbg* h){
    printf("Print hash drbg state:\n");
    printf("V:\n");
    print_hex(h->v, SEEDLEN/8);

    printf("C:\n");
    print_hex(h->c, SEEDLEN/8);

    printf("reseed counter:\n");
    printf("%zu\n", h->reseed_counter);
}

void plus_one(uint8_t* a, size_t len){
    uint8_t carry = 0;
    uint8_t tmp = a[len-1] + 1;
    carry = tmp < a[len-1];
    a[len-1] = tmp;
    for(int i=len-2; i>=0; i--){
        tmp = a[i] + carry;
        carry = tmp < a[i];
        a[i] = tmp;
    }
}

void static hash_gen(uint8_t* hash, uint8_t* v, size_t requested_no_of_bits){
    size_t m = (requested_no_of_bits/OUTLEN) + ((requested_no_of_bits % OUTLEN) >0);
    uint8_t * out_w = malloc(sizeof(uint8_t) * m * (OUTLEN/8));
    if (out_w == 0) return;
    int i =0;
    for(i=0;i<m * (OUTLEN/8);i++){
        out_w[i] = 0;
    }
    uint8_t *data = malloc(sizeof(uint8_t) * SEEDLEN/8);
    if (data ==0) return;
    for(i=0; i< SEEDLEN/8; i++){
        data[i] = v[i];
    }
    for(i=0; i <m; i++){
        sha256_8u_hash(out_w+((i)* (OUTLEN/8)),data,SEEDLEN);
        plus_one(data, SEEDLEN/8);
    }
    for(i=0;i<requested_no_of_bits/8;i++){
        hash[i] = out_w[i];
    }
    free(out_w); free(data); out_w=0; data=0;
}

void static hash_df(uint8_t* hash, size_t no_of_bits_to_return, const uint8_t* input_string, size_t ilen){
    size_t len = SEEDLEN/OUTLEN + (SEEDLEN%OUTLEN > 0);
    size_t hlen =1+4+ilen;
    size_t tmplen=len * (OUTLEN/8);
    uint8_t counter = 1;
    int i =0; int j =0;
    uint8_t* h = malloc(sizeof(uint8_t)*(hlen));
    if (h==0) return;
    uint8_t* tmp = malloc(sizeof(uint8_t)* tmplen);
    if (tmp==0) return;
    for(i=0;i<hlen;i++){
        h[i] = 0;
    }
    for(i=0;i< len * (OUTLEN/8); i++){
        tmp[i] = 0;
    }
    for(i=0;i<len;i++){
        h[0] = counter & 0xff;
        h[1] = (no_of_bits_to_return>>24) & 0xff;
        h[2] = (no_of_bits_to_return>>16) & 0xff;
        h[3] = (no_of_bits_to_return>>8) & 0xff;
        h[4] = (no_of_bits_to_return>>0) & 0xff;
        for(j=0; j<ilen; j++){
            h[j+5] = input_string[j];
        }
        print_hex(h, hlen);
        sha256_8u_hash(tmp+((i)* (OUTLEN/8)),h,hlen*8);
        counter++;
    }
    for(j=0,i=0; i<(no_of_bits_to_return/8) ;j++,i++){
        hash[i] = tmp[j];
    }
    free(h);h=0;free(tmp); tmp=0;
}

void hashdrbg_reseed(struct hashdrbg* h, const uint8_t* entropy_input, size_t elen, const uint8_t* additional_input, size_t alen){
    uint8_t* seed_material = malloc(sizeof(uint8_t)*((SEEDLEN/8)+elen + alen + 1));
    if (seed_material==0) return;
    seed_material[0] = 1;
    int i;
    for(i =0; i<SEEDLEN/8; i++){
        seed_material[i+1] = h->v[i];
    }
    for(i =0; i<elen; i++){
        seed_material[i+1+ SEEDLEN/8] = entropy_input[i];
    }
    for(i =0; i<alen; i++){
        seed_material[i+1+ SEEDLEN/8+elen] = additional_input[i];
    }
    hash_df(h->v, SEEDLEN, seed_material, (SEEDLEN/8)+elen+alen+1);
    uint8_t *tmp = malloc(sizeof(uint8_t) * ((SEEDLEN/8)+1));
    if (tmp==0) return;
    tmp[0] = 0;
    for(i=0; i < (SEEDLEN/8);i++){
        tmp[i+1] = h->v[i];
    }
    hash_df(h->c, SEEDLEN, tmp, (SEEDLEN/8)+1);
    h->reseed_counter = 1;
    print_state(h);
}

uint8_t* hashdrbg_next(struct hashdrbg* h, size_t requested_no_of_bit, const uint8_t* additional_input, size_t alen){
    int i =0; int j=0; uint8_t t =0; uint8_t carry =0; 
    if(additional_input){
        uint8_t* tmp2 = malloc(sizeof(uint8_t) * (SEEDLEN/8 + alen +1));
        if (tmp2==0) return 0;
        uint8_t* w = malloc(sizeof(uint8_t) * OUTLEN/8);
        if (w==0) return 0;
        tmp2[0]=2;
        for(i=0;i<SEEDLEN/8;i++){
            tmp2[1+i]=h->v[i];
        }
        for(i=0;i<alen;i++){
            tmp2[1+SEEDLEN/8+i] = additional_input[i];
        }
        sha256_8u_hash(w, tmp2, (SEEDLEN/8 + alen +1)*8);
        carry=0;
        for(i=(SEEDLEN/8)-1, j=OUTLEN/8-1; i>=0; i--, j--){
            if(j<0){
                t = h->v[i] + carry;
                carry = (t < h->v[i]);
            }else{
                t = h->v[i] + w[j] + carry;
                carry = (t < h->v[i]) || (t < w[j]);
            }
            h->v[i] = t;
        }
    }
    uint8_t* return_bits = malloc(sizeof(uint8_t) * (requested_no_of_bit/8));
    if (return_bits ==0) return 0;
    hash_gen(return_bits, h->v, requested_no_of_bit);
    uint8_t* tmp = malloc(sizeof(uint8_t) * ((SEEDLEN/8)+1));
    if (tmp ==0) return 0;
    uint8_t* hash   = malloc(sizeof(uint8_t) * OUTLEN/8);
    if (hash==0) return 0;
    tmp[0] = 3;
    for(i=0;i<SEEDLEN/8;i++){
        tmp[1+i] = h->v[i];
    }
    sha256_8u_hash(hash, tmp, ((SEEDLEN/8)+1)*8);
   
    t = h->v[(SEEDLEN/8)-1] + h->reseed_counter;
    carry = (t < h->v[(SEEDLEN/8)-1]) || (t < h->reseed_counter);
    h->v[(SEEDLEN/8)-1] = t;
    for(i=(SEEDLEN/8)-2; i>=0; i--){
        t = h->v[i] + carry;
        carry = (t < h->v[i]);
        h->v[i] = t;
    }
    carry=0;
    for(i=(SEEDLEN/8)-1; i>=0; i--){
        t = h->v[i] + h->c[i] + carry;
        carry = (t < h->v[i]) || (t < h->c[i]);
        h->v[i] = t;
    }
    carry=0;
    for(i=(SEEDLEN/8)-1, j=OUTLEN/8-1; i>=0; i--, j--){
        if(j<0){
            t = h->v[i] + carry;
            carry = (t < h->v[i]);
        }else{
            t = h->v[i] + hash[j] + carry;
            carry = (t < h->v[i]) || (t < hash[j]);
        }
        h->v[i] = t;
    }
    h->reseed_counter++;
    print_state(h);
    free(hash); free(tmp); hash=0; tmp=0;
    return return_bits;
}

struct hashdrbg* hashdrbg_init(const uint8_t* entropy_input, size_t elen, const uint8_t* nonce, size_t nlen){
    struct hashdrbg* h = malloc(sizeof(struct hashdrbg));
    if (h==0) return 0;
    h->v = malloc(sizeof(uint8_t) * SEEDLEN/8);
    if (h->v == 0) return 0;
    h->c = malloc(sizeof(uint8_t) * SEEDLEN/8);
    if (h->c == 0) return 0;
    uint8_t* seed_material = malloc(sizeof(uint8_t)*(elen + nlen));
    if (seed_material == 0) return 0;
    int i;
    for(i=0; i< SEEDLEN/8; i++){
        h->v[i] = 0;
        h->c[i] = 0;
    }
    h->reseed_counter = 1;
    for(i=0 ;i<elen;i++){
        seed_material[i] = entropy_input[i];
    }
    for(i=0;i<nlen;i++){
        seed_material[i+elen] = nonce[i];
    }
    hash_df(h->v, SEEDLEN, seed_material, elen+nlen);
    uint8_t *tmp = malloc(sizeof(uint8_t) * (SEEDLEN/8 + 1));
    if (tmp == 0) return 0;
    tmp[0] = 0;
    for(i=0; i<SEEDLEN/8; i++){
        tmp[i+1] = h->v[i];
    }
    hash_df(h->c, SEEDLEN, tmp, SEEDLEN/8 + 1);
    print_state(h);
    free(seed_material);seed_material = 0;
    free(tmp); tmp=0;
    return h;
}