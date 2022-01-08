#include "sha256_8u.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define D if(SHA256_8U_DEBUG)


bool SHA256_8U_DEBUG = false;

const static uint8_t ih[8][4] = {
    {0x6a,0x09,0xe6,0x67}, 
    {0xbb,0x67,0xae,0x85}, 
    {0x3c,0x6e,0xf3,0x72}, 
    {0xa5,0x4f,0xf5,0x3a}, 
    {0x51,0x0e,0x52,0x7f}, 
    {0x9b,0x05,0x68,0x8c}, 
    {0x1f,0x83,0xd9,0xab}, 
    {0x5b,0xe0,0xcd,0x19}
};

const static uint8_t k[64][4] = {
    {0x42,0x8a,0x2f,0x98}, {0x71,0x37,0x44,0x91}, {0xb5,0xc0,0xfb,0xcf}, {0xe9,0xb5,0xdb,0xa5}, {0x39,0x56,0xc2,0x5b}, {0x59,0xf1,0x11,0xf1}, {0x92,0x3f,0x82,0xa4}, {0xab,0x1c,0x5e,0xd5},
    {0xd8,0x07,0xaa,0x98}, {0x12,0x83,0x5b,0x01}, {0x24,0x31,0x85,0xbe}, {0x55,0x0c,0x7d,0xc3}, {0x72,0xbe,0x5d,0x74}, {0x80,0xde,0xb1,0xfe}, {0x9b,0xdc,0x06,0xa7}, {0xc1,0x9b,0xf1,0x74},
    {0xe4,0x9b,0x69,0xc1}, {0xef,0xbe,0x47,0x86}, {0x0f,0xc1,0x9d,0xc6}, {0x24,0x0c,0xa1,0xcc}, {0x2d,0xe9,0x2c,0x6f}, {0x4a,0x74,0x84,0xaa}, {0x5c,0xb0,0xa9,0xdc}, {0x76,0xf9,0x88,0xda},
    {0x98,0x3e,0x51,0x52}, {0xa8,0x31,0xc6,0x6d}, {0xb0,0x03,0x27,0xc8}, {0xbf,0x59,0x7f,0xc7}, {0xc6,0xe0,0x0b,0xf3}, {0xd5,0xa7,0x91,0x47}, {0x06,0xca,0x63,0x51}, {0x14,0x29,0x29,0x67},
    {0x27,0xb7,0x0a,0x85}, {0x2e,0x1b,0x21,0x38}, {0x4d,0x2c,0x6d,0xfc}, {0x53,0x38,0x0d,0x13}, {0x65,0x0a,0x73,0x54}, {0x76,0x6a,0x0a,0xbb}, {0x81,0xc2,0xc9,0x2e}, {0x92,0x72,0x2c,0x85},
    {0xa2,0xbf,0xe8,0xa1}, {0xa8,0x1a,0x66,0x4b}, {0xc2,0x4b,0x8b,0x70}, {0xc7,0x6c,0x51,0xa3}, {0xd1,0x92,0xe8,0x19}, {0xd6,0x99,0x06,0x24}, {0xf4,0x0e,0x35,0x85}, {0x10,0x6a,0xa0,0x70},
    {0x19,0xa4,0xc1,0x16}, {0x1e,0x37,0x6c,0x08}, {0x27,0x48,0x77,0x4c}, {0x34,0xb0,0xbc,0xb5}, {0x39,0x1c,0x0c,0xb3}, {0x4e,0xd8,0xaa,0x4a}, {0x5b,0x9c,0xca,0x4f}, {0x68,0x2e,0x6f,0xf3},
    {0x74,0x8f,0x82,0xee}, {0x78,0xa5,0x63,0x6f}, {0x84,0xc8,0x78,0x14}, {0x8c,0xc7,0x02,0x08}, {0x90,0xbe,0xff,0xfa}, {0xa4,0x50,0x6c,0xeb}, {0xbe,0xf9,0xa3,0xf7}, {0xc6,0x71,0x78,0xf2}
};

void static print_arr(uint8_t* rk, unsigned int len){
    for(int i =0; i<len; i++){
        printf("%02x ", rk[i]);
    }
    puts("");
}

int static pad(uint8_t * m, size_t pm_len, size_t start_pad_ind){
    size_t start_byte = (start_pad_ind+1)/8;
    for(size_t i = start_byte; i<pm_len; ++i){
        if(i == start_byte){
            uint8_t offset = (start_pad_ind+1)%8;
            *(m+i) &= (~0 << (7-offset+1));
            *(m+i) |= (1 << (7-offset+1));
        }else{
            *(m+i) = 0U;
        }
    }
    return 1;
}

int static pad_m_len(uint8_t * m, size_t pm_len, size_t m_len){
    for(uint8_t i =0;i<8;i++){
        *(m+(pm_len-i-1)) = m_len>>(i*8) & 0xff;
    }
    return 1;
}

void static copy(uint8_t* to, const uint8_t* from, size_t len){
    for(size_t i =0; i< len; i++){
        to[i] = from[i];
    }
}

void static rotr(uint8_t* x, size_t shift){
    uint8_t tmp[4] ={0};
    for(size_t i=0; i<32; i++){
        uint8_t ind = (i+shift) % (32);
        uint8_t bit = (x[3-(ind/8)] >> (ind%8)) & 1u;
        tmp[3-(i/8)] |= bit<<(i%8); 
    } 
    copy(x,tmp,4);
 }

 void static x_or(uint8_t* x, const uint8_t* y, size_t len){
     for(size_t i =0; i< len; i++){
         x[i] ^= y[i];
     }
 }

void static w_and(uint8_t* x, const uint8_t* y, size_t len){
     for(size_t i =0; i< len; i++){
         x[i] &= y[i];
     }
 }

 void static w_neg(uint8_t* x, size_t len){
     for(size_t i =0; i< len; i++){
         x[i] = ~x[i];
     }
 }

 void static w_add(uint8_t* x,const uint8_t* y,size_t len){
     uint8_t carry = 0;
     uint8_t tmp = 0;
     for(int i =len-1; i>=0; --i){
         tmp = x[i] + y[i] + carry;
         carry = (tmp < x[i]) || (tmp < y[i]) ;
         x[i] = tmp&0xff;
     }
 }

 void static rshift(uint8_t* x, size_t shift){
    uint8_t tmp[4] ={0};
    for(size_t i=0; i<32-shift; i++){
        uint8_t ind = i+shift;
        uint8_t bit = (x[3-(ind/8)] >> (ind%8)) & 1u;
        tmp[3-(i/8)] |= bit<<(i%8); 
    } 
    copy(x,tmp,4);
 }

void static func_s0(uint8_t* x){
    //return rotr(x,7) ^ rotr(x,18) ^ (x>>3);
    uint8_t tmp1[4] ={0}; uint8_t tmp2[4] ={0};
    copy(tmp1,x,4); copy(tmp2,x,4);
    rotr(x,7); rotr(tmp1,18); rshift(tmp2,3);
    x_or(x,tmp1,4); x_or(x,tmp2,4);
}

void static func_s1(uint8_t* x){
    //return rotr(x,17) ^ rotr(x,19) ^ (x>>10);
    uint8_t tmp1[4] ={0}; uint8_t tmp2[4] ={0};
    copy(tmp1,x,4); copy(tmp2,x,4);
    rotr(x,17); rotr(tmp1,19); rshift(tmp2,10);
    x_or(x,tmp1,4); x_or(x,tmp2,4);
}

void static func_e0(uint8_t* x){
    //return rotr(x, 2)^rotr(x, 13)^rotr(x,22);
    uint8_t tmp1[4] ={0}; uint8_t tmp2[4] ={0};
    copy(tmp1,x,4); copy(tmp2,x,4);
    rotr(x,2); 
    rotr(tmp1,13); rotr(tmp2,22);
    x_or(x,tmp1,4); x_or(x,tmp2,4);
}

void static func_e1(uint8_t* x){
    //return rotr(x, 6)^rotr(x, 11)^rotr(x,25);   
    uint8_t tmp1[4] ={0}; uint8_t tmp2[4] ={0};
    copy(tmp1,x,4); copy(tmp2,x,4);
    rotr(x,6); rotr(tmp1,11); rotr(tmp2,25);
    x_or(x,tmp1,4); x_or(x,tmp2,4);
}

void static func_ch(uint8_t* x, const uint8_t* y, const uint8_t* z){
    //return (((x) & (y)) ^ (~(x) & (z)));
    uint8_t tmp1[4] ={0};  copy(tmp1,x,4);
    w_and(x,y,4);
    w_neg(tmp1,4);w_and(tmp1,z,4);
    x_or(x,tmp1,4);
}

void static func_maj(uint8_t* x, const uint8_t* y, const uint8_t* z){
    //return (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)));
    uint8_t tmp1[4] ={0};uint8_t tmp2[4] ={0};
    copy(tmp1,x,4);copy(tmp2,y,4);
    w_and(x,y,4); w_and(tmp1,z,4); w_and(tmp2,z,4);
    x_or(x,tmp1,4);x_or(x,tmp2,4);
}

void static init_w(uint8_t* w, const uint8_t* m){
    uint8_t tmp1[4] ={0};
    uint8_t tmp2[4] ={0};
    uint8_t tmp3[4] ={0};
    uint8_t tmp4[4] ={0};
    for(size_t i=0; i<64; ++i){
        w[i]=m[i];
    }
    for(size_t i=64;i<256;i=i+4){
        //w[i] = func_s1(w[i-2]) + w[i-7] + func_s0(w[i-15]) + w[i-16];
        copy(tmp1, &w[i-(2*4)],4); func_s1(tmp1);
        copy(tmp2, &w[i-(7*4)],4); 
        copy(tmp3, &w[i-(15*4)],4); func_s0(tmp3);
        copy(tmp4, &w[i-(16*4)],4);
        copy(&w[i], tmp1,4);
        w_add(&w[i], tmp2,4);w_add(&w[i], tmp3,4);w_add(&w[i], tmp4,4);
    }
}

void static init_h(uint8_t* h){
    for(uint8_t i=0;i<8;i++){
        for(uint8_t j=0;j<4;j++){
            *(h+(4*i)+j) = ih[i][j];
        }
    }
}

int sha256_8u_hash(uint8_t* out, const uint8_t* m, size_t len ){
    //uint32_t a=ih[0], b=ih[1], c=ih[2], d=ih[3], e=ih[4], f=ih[5], g=ih[6], h=ih[7];
    uint8_t a[4]={0}; uint8_t b[4]={0}; uint8_t c[4]={0}; uint8_t d[4]={0}; 
    uint8_t e[4]={0}; uint8_t f[4]={0}; uint8_t g[4]={0}; uint8_t h[4]={0};
    copy(a,ih[0],4);copy(b,ih[1],4);copy(c,ih[2],4);copy(d,ih[3],4);
    copy(e,ih[4],4);copy(f,ih[5],4);copy(g,ih[6],4);copy(h,ih[7],4); 
 
    //size_t len = strlen((char*)m);
    D{printf(" Message : %s\n", (char*)m);}
    D{printf(" Message length: %zu\n", len);}

    size_t m_bit_len = len;
    D{printf(" Message bit length: %zu\n", m_bit_len);}
    size_t byte_pad_len = ((448 + 512)-(m_bit_len+1)%512)%512;
    D{printf(" Message pad bit length: %zu\n", byte_pad_len+1);}
    size_t padded_m_len = (len+byte_pad_len+1)/8+8;
    D{printf(" Padded message len: %zu\n", padded_m_len);}
    uint8_t * padded_m = malloc(sizeof(uint8_t)* padded_m_len);
    for(size_t i =0; i< len; ++i){
        *(padded_m+i) = m[i];
    }
    pad(padded_m, padded_m_len, len);
    D{printf(" Padded message: \n");}
    D{print_arr(padded_m, padded_m_len);}
    pad_m_len(padded_m, padded_m_len, m_bit_len);
    D{printf(" Padded message with message size: \n");}
    D{print_arr(padded_m, padded_m_len);}

    uint8_t t1[4]={0}; uint8_t t2[4]={0};
    uint8_t tmp[4]={0};
    uint8_t hash[8][4] = {0};
    uint8_t w[256] = {0};
    init_h(hash[0]);
    for (uint8_t round =0; round<padded_m_len/64; ++round){
        D{printf("Round=%u\n", round);}
        init_w(w, padded_m+(round*64));
        D{printf(" init w: \n");} 
        D{print_arr(w, 256);}
        //a=hash[0];b=hash[1];c=hash[2];d=hash[3];e=hash[4];f=hash[5];g=hash[6];h=hash[7];
        copy(a,hash[0],4);copy(b,hash[1],4);copy(c,hash[2],4);copy(d,hash[3],4);
        copy(e,hash[4],4);copy(f,hash[5],4);copy(g,hash[6],4);copy(h,hash[7],4);
        
        for(uint8_t i=0; i<64; ++i){
            //t1 = h + func_e1(e) + func_ch(e,f,g) + k[i] + w[i];
            copy(t1,h,4);copy(tmp,e,4); func_e1(tmp);w_add(t1,tmp,4);
            copy(tmp,e,4);func_ch(tmp,f,g); w_add(t1, tmp,4);
            w_add(t1, k[0]+(i*4),4);w_add(t1,w+(i*4),4);
            //t2 = func_e0(a) + func_maj(a,b,c);
            copy(t2, a,4); func_e0(t2);
            copy(tmp,a,4);func_maj(tmp,b,c);
            w_add(t2,tmp,4);
            //h=g; g=f; f=e; e=d+t1; d=c; c=b; b=a; a=t1+t2;
            copy(h,g,4);copy(g,f,4);copy(f,e,4);
            copy(e,d,4);w_add(e,t1,4);
            copy(d,c,4);copy(c,b,4);copy(b,a,4);
            copy(a,t1,4);w_add(a,t2,4);
        }
        D{printf(" a: \n");print_arr(a, 4);}
        D{printf(" b: \n");print_arr(b, 4);}
        D{printf(" c: \n");print_arr(c, 4);}
        D{printf(" d: \n");print_arr(d, 4);}
        D{printf(" e: \n");print_arr(e, 4);}
        D{printf(" f: \n");print_arr(f, 4);}
        D{printf(" g: \n");print_arr(g, 4);}
        D{printf(" h: \n");print_arr(h, 4);}

        //hash[0] +=a; hash[1] +=b; hash[2] +=c; hash[3] +=d; hash[4] +=e; hash[5] +=f; hash[6] +=g; hash[7] +=h;
        w_add(hash[0],a,4); w_add(hash[1],b,4); w_add(hash[2],c,4); w_add(hash[3],d,4);
        w_add(hash[4],e,4); w_add(hash[5],f,4); w_add(hash[6],g,4); w_add(hash[7],h,4);
        D{printf(" hash0: \n");print_arr(hash[0], 4);}
        D{printf(" hash1: \n");print_arr(hash[1], 4);}
        D{printf(" hash2: \n");print_arr(hash[2], 4);}
        D{printf(" hash3: \n");print_arr(hash[3], 4);}
        D{printf(" hash4: \n");print_arr(hash[4], 4);}
        D{printf(" hash5: \n");print_arr(hash[5], 4);}
        D{printf(" hash6: \n");print_arr(hash[6], 4);}
        D{printf(" hash7: \n");print_arr(hash[7], 4);}
    }
    for(uint8_t i=0;i<8;++i){
        for(uint8_t j=0;j<4;++j){
            out[i*4+j] = hash[i][j];
        }
    }
    return 1;
}