#include "des.h"
#include <stdio.h>
#define BIT(x)  (1 << x)

bool DES_DEBUG = false;

static const unsigned int ip_matrix[64]={
    58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,
    62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,
    57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,
    61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7
};

static const unsigned int key_permutation_matrix[56]={
    57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 
    10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 
    14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
};

static const unsigned int key_rotate_1_matrix[56]={
    28,1,2,3,4,5,6,7,
    8,9,10,11,12,13,14,15,
    16,17,18,19,20,21,22,23,
    24,25,26,27,56,29,30,31,
    32,33,34,35,36,37,38,39,
    40,41,42,43,44,45,46,47,
    48,49,50,51,52,53,54,55
};

static const unsigned int key_rotate_2_matrix[56]={
    27,28,1,2,3,4,5,6,
    7,8,9,10,11,12,13,14,
    15,16,17,18,19,20,21,22,
    23,24,25,26,55,56,29,30,
    31,32,33,34,35,36,37,38,
    39,40,41,42,43,44,45,46,
    47,48,49,50,51,52,53,54
};

static const unsigned int key_compress_matrix[48]={
    14,17,11,24,1,5,3,28,15,6,21,10,
    23,19,12,4,26,8,16,7,27,20,13,2,
    41,52,31,37,47,55,30,40,51,45,33,48,
    44,49,39,56,34,53,46,42,50,36,29,32
};

static const unsigned int exp_matrix[48]={
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9,
    10, 11, 12, 13, 12, 13, 14, 15, 16, 17, 16,
    17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
};

static const unsigned int sbox1[4][16]={
    14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7, 
    0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8, 
    4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0, 
    15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
};
static const unsigned int sbox2[4][16]={
    15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10, 
    3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5, 
    0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15, 
    13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
};
static const unsigned int sbox3[4][16]={
    10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8, 
    13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1, 
    13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7, 
    1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
};
static const unsigned int sbox4[4][16]={
    7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15, 
    13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9, 
    10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4, 
    3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
};
static const unsigned int sbox5[4][16]={
    2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9, 
    14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6, 
    4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14, 
    11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
};
static const unsigned int sbox6[4][16]={
    12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11, 
    10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8, 
    9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6, 
    4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
};
static const unsigned int sbox7[4][16]={
    4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1, 
    13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6, 
    1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2, 
    6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
};
static const unsigned int sbox8[4][16]={
    13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7, 
    1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2, 
    7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8, 
    2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
};

static const unsigned int key_rotate_seq[16] = {1 ,1 ,2 ,2 ,2 ,2 ,2 ,2 ,1 ,2 ,2 ,2 ,2 ,2 ,2 ,1 };

static const unsigned int pbox_matrix[32] = {
    16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
};

static const unsigned int fp_matrix[64] ={
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31, 
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29, 
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
};

void print_arr(unsigned char* rk, unsigned int len){
    for(int i =len-1; i>=0; i--){
        printf("%x ", rk[i]);
    }
    puts("");
}

void printBits(size_t const size, void const * const ptr){
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
    }
    puts("");
}

int ip(unsigned char* pt){
    unsigned char tmp[8];
    for (int i=0; i<8; i++){
        tmp[i] = pt[i];
    }
    for (int i=0; i<64; i++){
        unsigned int ind = 64U - ip_matrix[i];
        unsigned char bit = (tmp[ind/8] >> ind%8) & 1U;
        pt[7-(i/8)] = (pt[7-(i/8)] & ~(1U << (7-(i%8)))) | (bit << (7-(i%8)));
    }
    return 0;
}

int fp(unsigned char* pt){
    unsigned char tmp[8];
    for (int i=0; i<8; i++){
        tmp[i] = pt[i];
    }
    for (int i=0; i<64; i++){
        unsigned char bit = (tmp[(fp_matrix[i]-1)/8] >> (fp_matrix[i]-1)%8) & 1U;
        pt[i/8] = (pt[i/8] & ~(1U << (i%8))) | (bit << (i%8));
    }
    return 0;
}


int key_permutation(unsigned char * pk, const unsigned char * k){
    for (int i=0; i<56; i++){
        unsigned int ind = 64U - key_permutation_matrix[i];
        unsigned char bit = (k[ind/8] >> ind%8) & 1U;
        pk[6-(i/8)] = (pk[6-(i/8)] & ~(1U << (7-(i%8)))) | (bit << (7-(i%8)));
    }
    return 0;
}

int key_rotate_1(unsigned char * k){
    unsigned char tmp[7];
    for (int i=0; i<7; i++){
        tmp[i] = k[i];
    }
    for (int i=0; i<56; i++){
        unsigned char bit = (tmp[(key_rotate_1_matrix[i]-1)/8] >> (key_rotate_1_matrix[i]-1)%8) & 1U;
        k[i/8] = (k[i/8] & ~(1U << (i%8))) | (bit << (i%8));
    }
    return 0;
}

int key_rotate_2(unsigned char * k){
    unsigned char tmp[7];
    for (int i=0; i<7; i++){
        tmp[i] = k[i];
    }
    for (int i=0; i<56; i++){
        unsigned char bit = (tmp[(key_rotate_2_matrix[i]-1)/8] >> (key_rotate_2_matrix[i]-1)%8) & 1U;
        k[i/8] = (k[i/8] & ~(1U << (i%8))) | (bit << (i%8));
    }
    return 0;
}

int key_compress(unsigned char * ck, unsigned char* k){
    for (int i=0; i<48; i++){
        unsigned int ind = 56U - key_compress_matrix[i];
        unsigned char bit = (k[ind/8] >> ind%8) & 1U;
        ck[5-(i/8)] = (ck[5-(i/8)] & ~(1U << (7-(i%8)))) | (bit << (7-(i%8)));
    }
    return 0;
}

int expansion(unsigned char * em, unsigned char* m){
    for (int i=0; i<48; i++){
        unsigned int ind = 32U - exp_matrix[i];
        unsigned char bit = (m[ind/8] >> ind%8) & 1U;
        em[5-(i/8)] = (em[5-(i/8)] & ~(1U << (7-(i%8)))) | (bit << (7-(i%8)));
    }
    return 0;
}

unsigned int sbox(unsigned int box_num, unsigned char input){
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

int sbox_m(unsigned char* m){
    unsigned char sbox_tmp[8];
    for(int i=7 ; i>=0; i--){
        unsigned char bit1 = (m[(i*6+0)/8] >> (i*6+0)%8) & 1U;
        unsigned char bit2 = (m[(i*6+1)/8] >> (i*6+1)%8) & 1U;
        unsigned char bit3 = (m[(i*6+2)/8] >> (i*6+2)%8) & 1U;
        unsigned char bit4 = (m[(i*6+3)/8] >> (i*6+3)%8) & 1U;
        unsigned char bit5 = (m[(i*6+4)/8] >> (i*6+4)%8) & 1U;
        unsigned char bit6 = (m[(i*6+5)/8] >> (i*6+5)%8) & 1U;
        unsigned char input = bit1|(bit2<<1)|(bit3<<2)|(bit4<<3)|(bit5<<4)|(bit6<<5);
        sbox_tmp[i] = sbox(8-i, input);
    }
    if(DES_DEBUG){
        printf("sbox tmp :\n");
        printBits(8, sbox_tmp);
    }
    for(int ind=31; ind>=0; ind--){
        unsigned char bit = (sbox_tmp[ind/4] >> (ind%4)) & 1U;
        m[ind/8] = (m[ind/8] & ~(1U << (ind%8))) | (bit << (ind%8));
    }
    return 0;
}

int pbox(unsigned char* m){
    unsigned char tmp[4];
    for (int i=0; i<4; i++){
        tmp[i] = m[i];
    }
    for (int i=0; i<32; i++){
        unsigned int ind = 32U - pbox_matrix[i];
        unsigned char bit = (tmp[ind/8] >> ind%8) & 1U;
        m[3-(i/8)] = (m[3-(i/8)] & ~(1U << (7-(i%8)))) | (bit << (7-(i%8)));
    }
    return 0;
}

int des_core(unsigned char* cipher, const unsigned char* key, const unsigned char* pt, unsigned int mode){
    //init
    unsigned char ptc[8]={0};
    unsigned char keyc[7]={0};
    for(int i=0; i<8; i++){
        ptc[i] = pt[i];
    }
    if(DES_DEBUG){
        puts("Before key permutation");
        printBits(8, key);
    }
    key_permutation(keyc, key);
    if(DES_DEBUG){
        puts("After key permutation");
        printBits(7, keyc);
    }
    unsigned char left[4]={0};
    unsigned char right[4]={0};
    unsigned char round_key[16][6]={0};
    unsigned char exp_m[6]={0};
    //initial permutation
    if(DES_DEBUG){
        puts("Before m permutation");
        printBits(8, ptc);
    }
    ip(ptc);
    if(DES_DEBUG){
        puts("After m permutation");
        printBits(8, ptc);
    }
    //split left and right
    for(int i=0; i<4; i++){
        left[i]=ptc[i+4];
    }
    for(int i=0; i<4; i++){
        right[i] = ptc[i];
    }
    //Init round key
    for(int round=0; round<16; round++){
        if(key_rotate_seq[round]==1){
            key_rotate_1(keyc);
        }else if (key_rotate_seq[round]==2){  
            key_rotate_2(keyc);
        }
        if(DES_DEBUG){
            printf("Rotate Key %d:\n",round);
            printBits(7, keyc);
        }
        key_compress(round_key[round], keyc);
        if(DES_DEBUG){
            printf("Round Key %d:\n",round);
            printBits(6, round_key[round]);
            print_arr(round_key[round],6);
        }
    }
    //DES inner round
    for(int round=0; round<16; round++){
        expansion(exp_m, right);
        if(DES_DEBUG){
            printf("left %d:\n",round);
            printBits(4, left);
            printf("right %d:\n",round);
            printBits(4, right);
            printf("Expansion %d:\n",round);
            printBits(6, exp_m);
        }
        //R XOR round_key 
        for(int i=0; i<6; i++){
            if(mode==0){
                exp_m[i] ^= round_key[round][i];
            }else{
                exp_m[i] ^= round_key[15-round][i];
            }
        }
        if(DES_DEBUG){
            printf("XOR Expansion %d:\n",round);
            printBits(6, exp_m);
        }
        sbox_m(exp_m);
        if(DES_DEBUG){
            printf("After sbox %d:\n",round);
            printBits(4, exp_m);
        }
        pbox(exp_m);
        if(DES_DEBUG){
            printf("After pbox %d:\n",round);
            printBits(4, exp_m);
        }

        unsigned char tmp[4];
        for(int i=0; i<4; i++){
            tmp[i] = left[i];
        }
        for(int i=0; i<4; i++){
            left[i]=right[i];
        }
        for(int i=0; i<4; i++){
            right[i]=tmp[i] ^ exp_m[i];
        }
    }
    if(DES_DEBUG){
        printf("left %d:\n",16);
        printBits(4, left);
        printf("right %d:\n",16);
        printBits(4, right);
    }
    //final permutation
    cipher[0] = left[0];
    cipher[1] = left[1];
    cipher[2] = left[2];
    cipher[3] = left[3];
    cipher[4] = right[0];
    cipher[5] = right[1];
    cipher[6] = right[2];
    cipher[7] = right[3];
    if(DES_DEBUG){
        printf("before permute Cipher :\n");
        printBits(8, cipher);
    }
    fp(cipher);
    if(DES_DEBUG){
        printf("after permute Cipher :\n");
        printBits(8, cipher);
    }
    return 0;
}

int des_encrypt(unsigned char* cipher, const unsigned char* key, const unsigned char* plaintext){
    des_core(cipher, key, plaintext, 0);
    return 0;
}

int des_decrypt(unsigned char* plaintext, const unsigned char* key, const unsigned char* cipher){
    des_core(plaintext, key, cipher, 1);
    return 0;
}