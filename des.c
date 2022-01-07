#include "des.h"
#include <stdio.h>
#include <stdint.h>
#define BIT(x)  (1 << x)
#define D if(DES_DEBUG)

bool DES_DEBUG = false;

static const uint8_t ip_matrix[64]={
    58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,
    62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,
    57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,
    61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7
};

static const uint8_t key_permutation_matrix[56]={
    57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 
    10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 
    14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
};

static const uint8_t key_rotate_1_matrix[56]={
    2,3,4,5,6,7,8,9,
    10,11,12,13,14,15,16,17,
    18,19,20,21,22,23,24,25,
    26,27,28,1,
    30,31,32,33,
    34,35,36,37,38,39,40,41,
    42,43,44,45,46,47,48,49,
    50,51,52,53,54,55,56,29
};

static const uint8_t key_rotate_2_matrix[56]={
    3,4,5,6,7,8,9,
    10,11,12,13,14,15,16,17,
    18,19,20,21,22,23,24,25,
    26,27,28,1,2,
    31,32,33,
    34,35,36,37,38,39,40,41,
    42,43,44,45,46,47,48,49,
    50,51,52,53,54,55,56,29,30
};

static const uint8_t key_compress_matrix[48]={
    14,17,11,24,1,5,3,28,15,6,21,10,
    23,19,12,4,26,8,16,7,27,20,13,2,
    41,52,31,37,47,55,30,40,51,45,33,48,
    44,49,39,56,34,53,46,42,50,36,29,32
};

static const uint8_t exp_matrix[48]={
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9,
    10, 11, 12, 13, 12, 13, 14, 15, 16, 17, 16,
    17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
};

static const uint8_t sbox1[4][16]={
    14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7, 
    0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8, 
    4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0, 
    15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
};
static const uint8_t sbox2[4][16]={
    15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10, 
    3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5, 
    0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15, 
    13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
};
static const uint8_t sbox3[4][16]={
    10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8, 
    13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1, 
    13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7, 
    1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
};
static const uint8_t sbox4[4][16]={
    7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15, 
    13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9, 
    10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4, 
    3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
};
static const uint8_t sbox5[4][16]={
    2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9, 
    14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6, 
    4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14, 
    11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
};
static const uint8_t sbox6[4][16]={
    12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11, 
    10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8, 
    9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6, 
    4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
};
static const uint8_t sbox7[4][16]={
    4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1, 
    13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6, 
    1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2, 
    6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
};
static const uint8_t sbox8[4][16]={
    13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7, 
    1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2, 
    7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8, 
    2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
};

static const uint8_t key_rotate_seq[16] = {1 ,1 ,2 ,2 ,2 ,2 ,2 ,2 ,1 ,2 ,2 ,2 ,2 ,2 ,2 ,1 };

static const uint8_t pbox_matrix[32] = {
    16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
};

static const uint8_t fp_matrix[64] ={
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31, 
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29, 
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
};

void print_arr(const uint8_t* rk, uint8_t len){
    for(int i =0; i<len; ++i){
        printf("%x ", rk[i]);
    }
    puts("");
}

void printBits(size_t const size, void const * const ptr){
    uint8_t *b = (uint8_t*) ptr;
    uint8_t byte;
    int i, j;
    
    for (i = 0; i < size; ++i) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
    }
    puts("");
}

int ip(uint8_t* pt){
    uint8_t tmp[8];
    for (uint8_t i=0; i<8; i++){
        tmp[i] = pt[i];
    }
    for (int i=0; i<64; i++){
        uint8_t ind = ip_matrix[i]-1;
        uint8_t bit = (tmp[ind/8] >> (8-ind%8-1)) & 1U;
        pt[(i/8)] = (pt[(i/8)] & ~(1U << (7-(i%8)))) | (bit << (7-(i%8)));
    }
    return 0;
}

int fp(uint8_t* pt){
    uint8_t tmp[8];
    for (uint8_t i=0; i<8; i++){
        tmp[i] = pt[i];
    }
    for (uint8_t i=0; i<64; i++){
        uint8_t ind = fp_matrix[i]-1;
        uint8_t bit = (tmp[ind/8] >> (8-ind%8-1)) & 1U;
        pt[(i/8)] = (pt[(i/8)] & ~(1U << (7-(i%8)))) | (bit << (7-(i%8)));
    }
    return 0;
}


int key_permutation(uint8_t * pk, const uint8_t * k){
    for (int i=0; i<56; i++){
        uint8_t ind = key_permutation_matrix[i]-1;
        uint8_t bit = (k[ind/8] >> (8-ind%8-1)) & 1U;
        pk[(i/8)] = (pk[(i/8)] & ~(1U << (7-(i%8)))) | (bit << (7-(i%8)));
    }
    return 0;
}

int key_rotate_1(uint8_t * k){
    uint8_t tmp[7];
    for (uint8_t i=0; i<7; i++){
        tmp[i] = k[i];
    }
    for (uint8_t i=0; i<56; i++){
        uint8_t bit = (tmp[(key_rotate_1_matrix[i]-1)/8] >> (7-(key_rotate_1_matrix[i]-1)%8)) & 1U;
        k[i/8] = (k[i/8] & ~(1U << (7-(i%8)))) | (bit << (7-(i%8)));
    }
    return 0;
}

int key_rotate_2(uint8_t * k){
    uint8_t tmp[7];
    for (uint8_t i=0; i<7; i++){
        tmp[i] = k[i];
    }
    for (uint8_t i=0; i<56; i++){
        uint8_t bit = (tmp[(key_rotate_2_matrix[i]-1)/8] >> (7-(key_rotate_2_matrix[i]-1)%8)) & 1U;
        k[i/8] = (k[i/8] & ~(1U << (7-(i%8)))) | (bit << (7-(i%8)));
    }
    return 0;
}

int key_compress(uint8_t * ck, uint8_t* k){
    for (uint8_t i=0; i<48; i++){
        uint8_t ind =  key_compress_matrix[i]-1;
        uint8_t bit = (k[ind/8] >> (8-ind%8-1)) & 1U;
        ck[(i/8)] = (ck[(i/8)] & ~(1U << (7-(i%8)))) | (bit << (7-(i%8)));
    }
    return 0;
}

int expansion(uint8_t * em, uint8_t* m){
    for (int i=0; i<48; i++){
        uint8_t ind = exp_matrix[i]-1;
        uint8_t bit = (m[ind/8] >> (8-ind%8-1)) & 1U;
        em[(i/8)] = (em[(i/8)] & ~(1U << (7-(i%8)))) | (bit << (7-(i%8)));
    }
    return 0;
}

uint8_t sbox(uint8_t box_num, uint8_t input){
    if(box_num == 1){
        return (sbox1[((input & BIT(5))?2:0)|( input & BIT(0))][(input >> 1) & 0xf]);                          
    }else if(box_num==2){
        return (sbox2[((input & BIT(5))?2:0)|( input & BIT(0))][(input >> 1) & 0xf]); 
    }else if(box_num==3){
        return (sbox3[((input & BIT(5))?2:0)|( input & BIT(0))][(input >> 1) & 0xf]); 
    }else if(box_num==4){
        return (sbox4[((input & BIT(5))?2:0)|( input & BIT(0))][(input >> 1) & 0xf]); 
    }else if(box_num==5){
        return (sbox5[((input & BIT(5))?2:0)|( input & BIT(0))][(input >> 1) & 0xf]); 
    }else if(box_num==6){
        return (sbox6[((input & BIT(5))?2:0)|( input & BIT(0))][(input >> 1) & 0xf]); 
    }else if(box_num==7){
        return (sbox7[((input & BIT(5))?2:0)|( input & BIT(0))][(input >> 1) & 0xf]); 
    }else if(box_num==8){
        return (sbox8[((input & BIT(5))?2:0)|( input & BIT(0))][(input >> 1) & 0xf]); 
    }
    return 0;
}

int sbox_m(uint8_t* m){
    uint8_t sbox_tmp[8];
    for(int i=0 ; i<8; ++i){
        uint8_t bit1 = (m[(i*6+0)/8] >> (7-(i*6+0)%8)) & 1U;
        uint8_t bit2 = (m[(i*6+1)/8] >> (7-(i*6+1)%8)) & 1U;
        uint8_t bit3 = (m[(i*6+2)/8] >> (7-(i*6+2)%8)) & 1U;
        uint8_t bit4 = (m[(i*6+3)/8] >> (7-(i*6+3)%8)) & 1U;
        uint8_t bit5 = (m[(i*6+4)/8] >> (7-(i*6+4)%8)) & 1U;
        uint8_t bit6 = (m[(i*6+5)/8] >> (7-(i*6+5)%8)) & 1U;
        uint8_t input = bit6|(bit5<<1)|(bit4<<2)|(bit3<<3)|(bit2<<4)|(bit1<<5);
        sbox_tmp[i] = sbox(i+1, input);
    }
    D{ printf("sbox tmp :\n"); printBits(8, sbox_tmp); }
    for(uint8_t ind=0; ind<32; ++ind){
        uint8_t bit = (sbox_tmp[ind/4] >> (3-(ind%4))) & 1U;
        m[ind/8] = (m[ind/8] & ~(1U << (7-(ind%8)))) | (bit << (7-(ind%8)));
    }
    return 0;
}

int pbox(uint8_t* m){
    uint8_t tmp[4];
    for (uint8_t i=0; i<4; i++){
        tmp[i] = m[i];
    }
    for (uint8_t i=0; i<32; i++){
        uint8_t ind =  pbox_matrix[i]-1;
        uint8_t bit = (tmp[ind/8] >> (8-ind%8-1)) & 1U;
        m[(i/8)] = (m[(i/8)] & ~(1U << (7-(i%8)))) | (bit << (7-(i%8)));
    }
    return 0;
}

int des_core(uint8_t* cipher, const uint8_t* key, const uint8_t* pt, uint8_t mode){
    //init
    uint8_t ptc[8]={0};
    uint8_t keyc[7]={0};
    for(int i=0; i<8; i++){
        ptc[i] = pt[i];
    }
    D{ puts("Before key permutation"); printBits(8, key); }
    key_permutation(keyc, key);
    D{ puts("After key permutation"); printBits(7, keyc); }
    uint8_t left[4]={0};
    uint8_t right[4]={0};
    uint8_t round_key[16][6]={0};
    uint8_t exp_m[6]={0};
    //initial permutation
    D{ puts("Before m permutation"); printBits(8, ptc); }
    ip(ptc);
    D{ puts("After m permutation"); printBits(8, ptc); }
    //split left and right
    for(uint8_t i=0; i<4; i++){
        left[i]=ptc[i];
    }
    for(uint8_t i=0; i<4; i++){
        right[i] = ptc[i+4];
    }
    //Init round key
    for(uint8_t round=0; round<16; round++){
        if(key_rotate_seq[round]==1){
            key_rotate_1(keyc);
        }else if (key_rotate_seq[round]==2){  
            key_rotate_2(keyc);
        }
        D{ printf("Rotate Key %d:\n",round); printBits(7, keyc); }
        key_compress(round_key[round], keyc);
        D{ printf("Round Key %d:\n",round); printBits(6, round_key[round]); print_arr(round_key[round],6); }
    }
    //DES inner round
    for(uint8_t round=0; round<16; round++){
        expansion(exp_m, right);
        D{ printf("left %d:\n",round); printBits(4, left); printf("right %d:\n",round); printBits(4, right); printf("Expansion %d:\n",round); printBits(6, exp_m); }
        //R XOR round_key 
        for(uint8_t i=0; i<6; i++){
            if(mode==0){
                exp_m[i] ^= round_key[round][i];
            }else{
                exp_m[i] ^= round_key[15-round][i];
            }
        }
        D{ printf("XOR Expansion %d:\n",round); printBits(6, exp_m); }
        sbox_m(exp_m);
        D{ printf("After sbox %d:\n",round); printBits(4, exp_m); }
        pbox(exp_m);
        D{ printf("After pbox %d:\n",round); printBits(4, exp_m); }

        uint8_t tmp[4];
        for(uint8_t i=0; i<4; i++){
            tmp[i] = left[i];
        }
        for(uint8_t i=0; i<4; i++){
            left[i]=right[i];
        }
        for(uint8_t i=0; i<4; i++){
            right[i]=tmp[i] ^ exp_m[i];
        }
    }
    D{ printf("left %d:\n",16); printBits(4, left); printf("right %d:\n",16); printBits(4, right); }
    //final permutation
    cipher[0] = right[0];
    cipher[1] = right[1];
    cipher[2] = right[2];
    cipher[3] = right[3];
    cipher[4] = left[0];
    cipher[5] = left[1];
    cipher[6] = left[2];
    cipher[7] = left[3];
    D{ printf("before permute Cipher :\n"); printBits(8, cipher); }
    fp(cipher);
    D{ printf("after permute Cipher :\n"); printBits(8, cipher); }
    return 0;
}

int des_encrypt(uint8_t* cipher, const uint8_t* key, const uint8_t* plaintext){
    des_core(cipher, key, plaintext, 0);
    return 0;
}

int des_decrypt(uint8_t* plaintext, const uint8_t* key, const uint8_t* cipher){
    des_core(plaintext, key, cipher, 1);
    return 0;
}