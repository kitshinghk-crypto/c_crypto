#include "aes.h"
#include <stdio.h>
#include <stdint.h>
#define D if(AES_DEBUG)

bool AES_DEBUG = false;

static const uint8_t rcon[10]={
    0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1B,0x36
};

static const uint8_t aes_sbox[256] = {
 /* 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F  */
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

static const uint8_t aes_inv_sbox[256] = {
  0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

void static print_arr(const uint8_t* rk, unsigned int len){
    for(int i =len-1; i>=0; i--){
        printf("%x ", rk[len-1-i]);
    }
    puts("");
}

void static printBits(size_t const size, void const * const ptr){
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

/**
** Key = {54 68 61 74 73 20 6D 79 20 4B 75 6E 67 20 46 75}
**         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
** unsigned char roundkey[11][16]
** W0 = (54, 68, 61, 74) = (roundkey[0][0] roundkey[0][1] roundkey[0][2] roundkey[0][3])
** W1 = (73, 20, 6D, 79) = (roundkey[0][4] roundkey[0][5] roundkey[0][6] roundkey[0][7])
** W2 = (20, 4B, 75, 6E) = (roundkey[0][8] roundkey[0][9] roundkey[0][10] roundkey[0][11])
** W3 = (67, 20, 46, 75) = (roundkey[0][12] roundkey[0][13] roundkey[0][14] roundkey[0][15])
**/
int static init_key_schedule(uint8_t* roundkey, const uint8_t* key){
    for(uint8_t i=0; i<16; i++){
        *(roundkey+0*16+i) = key[i];
    }
    for (uint8_t w=4; w<44; w++){
        uint8_t round = w/4;
        uint8_t offset = (w%4)*4;
        uint8_t prev_round = (w-1)/4;
        uint8_t prev_offset = ((w-1)%4)*4;
        //RotWord
        if(w%4==0){
            *(roundkey+round*16+offset+0)=*(roundkey+(prev_round)*16+prev_offset+1);
            *(roundkey+round*16+offset+1)=*(roundkey+(prev_round)*16+prev_offset+2);
            *(roundkey+round*16+offset+2)=*(roundkey+(prev_round)*16+prev_offset+3);
            *(roundkey+round*16+offset+3)=*(roundkey+(prev_round)*16+prev_offset+0);
        }else{
            *(roundkey+round*16+offset+0)=*(roundkey+(prev_round)*16+prev_offset+0);
            *(roundkey+round*16+offset+1)=*(roundkey+(prev_round)*16+prev_offset+1);
            *(roundkey+round*16+offset+2)=*(roundkey+(prev_round)*16+prev_offset+2);
            *(roundkey+round*16+offset+3)=*(roundkey+(prev_round)*16+prev_offset+3);
        }
        //SubWord
        if(w%4==0){
            *(roundkey+round*16+offset+0)=aes_sbox[*(roundkey+round*16+offset+0)];
            *(roundkey+round*16+offset+1)=aes_sbox[*(roundkey+round*16+offset+1)];
            *(roundkey+round*16+offset+2)=aes_sbox[*(roundkey+round*16+offset+2)];
            *(roundkey+round*16+offset+3)=aes_sbox[*(roundkey+round*16+offset+3)];
        }
        //XOR
        *(roundkey+round*16+offset+0)^=*(roundkey+(round-1)*16+offset+0);
        *(roundkey+round*16+offset+1)^=*(roundkey+(round-1)*16+offset+1);
        *(roundkey+round*16+offset+2)^=*(roundkey+(round-1)*16+offset+2);
        *(roundkey+round*16+offset+3)^=*(roundkey+(round-1)*16+offset+3);
        if(w%4==0){
            *(roundkey+round*16+offset+0)^= rcon[round-1];
        }
    }
    return 0;
}
uint8_t xtime(uint8_t x){
  return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

uint8_t static Multiply(uint8_t x, uint8_t y){
  return (((y & 1) * x) ^
       ((y>>1 & 1) * xtime(x)) ^
       ((y>>2 & 1) * xtime(xtime(x))) ^
       ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^
       ((y>>4 & 1) * xtime(xtime(xtime(xtime(x)))))); /* this last call to xtime() can be omitted */
}

int static add_roundkey(uint8_t* state, const uint8_t* roundkey){
    for(int i=0; i<16; i++){
        state[i]^=roundkey[i];
    }
    return 0;
}

int static sub_byte(uint8_t* state){
    for(int i=0; i<16; i++){
        state[i] = aes_sbox[state[i]];
    }
    return 0;
}

int static inv_sub_byte(uint8_t* state){
    for(int i=0; i<16; i++){
        state[i] = aes_inv_sbox[state[i]];
    }
    return 0;
}

int static shift_row(uint8_t* state){
    uint8_t tmp = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13];
    state[13] = tmp;

    tmp = state[2];
    state[2] = state[10];
    state[10] = tmp;
    tmp = state[14];
    state[14] = state[6];
    state[6] = tmp;
    
    
    tmp = state[3];
    state[3] = state[15];
    state[15] = state[11];
    state[11] = state[7];
    state[7] = tmp;
    return 0;
}

int static inv_shift_row(uint8_t* state){
    uint8_t tmp = state[1];
    state[1] = state[13];
    state[13] = state[9];
    state[9] = state[5];
    state[5] = tmp;

    tmp = state[2];
    state[2] = state[10];
    state[10] = tmp;
    tmp = state[14];
    state[14] = state[6];
    state[6] = tmp;
    
    
    tmp = state[3];
    state[3] = state[7];
    state[7] = state[11];
    state[11] = state[15];
    state[15] = tmp;
    return 0;
}

int static mix_one_col(uint8_t* col){
    uint8_t a[4]={0};
    uint8_t b[4]={0};
    uint8_t c;
    uint8_t h;
    for (c=0; c<4; c++){
        a[c] = col[c];
        h = (col[c] >> 7) & 1;
        b[c] = col[c] << 1;
        b[c] ^= h * 0x1B;
    }
    col[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1]; /* 2 * a0 + a3 + a2 + 3 * a1 */
    col[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2]; /* 2 * a1 + a0 + a3 + 3 * a2 */
    col[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3]; /* 2 * a2 + a1 + a0 + 3 * a3 */
    col[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0]; /* 2 * a3 + a2 + a1 + 3 * a0 */
    return 0;
}

int static mix_column(uint8_t* state){
    for(int i=0; i<16; i+=4){
        mix_one_col(&state[i]);
    }
    return 0;
}

int static inv_mix_one_col(uint8_t* col){
    uint8_t a, b, c, d;
    a = col[0];
    b = col[1];
    c = col[2];
    d = col[3];

    col[0] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
    col[1] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
    col[2] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
    col[3] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
    return 0;
}

int static inv_mix_column(uint8_t* state){
    for(int i=0; i<16; i+=4){
        inv_mix_one_col(&state[i]);
    }
    return 0;
}

/**
**  M = E2 32 FC F1 91 12 91 88 B1 59 E4 E6 D6 79 A2 93
**       0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
** 
**/
void aes_encrypt(uint8_t* cipher, const uint8_t* key, const uint8_t* plaintext){
    uint8_t state[16] = {0};
    for(int i =0; i<16; i++){
        state[i] = plaintext[i];
    }
    uint8_t roundKey[11][16] = {0};
    init_key_schedule(roundKey[0], key);
    D{ printf("init state: \n"); print_arr(state, 16); }
    add_roundkey(state, roundKey[0]);
    D{ printf("first add roundky state: \n"); print_arr(state, 16); }
    for(int i =1; i<10; i++){
        sub_byte(state);
        D{ printf("Round:%d subbyte state: \n", i); print_arr(state, 16); }
        shift_row(state);
        D{ printf("Round:%d shift row state: \n", i); print_arr(state, 16); }
        mix_column(state);
        D{ printf("Round:%d mix col state: \n", i); print_arr(state, 16); }
        add_roundkey(state, roundKey[i]);
        D{ printf("Round:%d add roundkey state: \n", i); print_arr(state, 16); }
    }
    sub_byte(state);
    D{ printf("Round:%d subbyte state: \n", 10); print_arr(state, 16); }
    shift_row(state);
    D{ printf("Round:%d shift row state: \n", 10); print_arr(state, 16); }
    add_roundkey(state, roundKey[10]);
    D{ printf("Round:%d add roundkey state: \n", 10); print_arr(state, 16); }
    for(int i = 0; i< 16; i++){
        cipher[i] = state[i];
    }
}

void aes_decrypt(uint8_t* plaintext, const uint8_t* key, const uint8_t* cipher){
    uint8_t state[16] = {0};
    for(int i =0; i<16; i++){
        state[i] = cipher[i];
    }
    uint8_t roundKey[11][16] = {0};
    init_key_schedule(roundKey[0], key);
    D{ printf("init state: \n"); print_arr(state, 16); }
    add_roundkey(state, roundKey[10]);
    D{ printf("first add roundky state: \n"); print_arr(state, 16); }
    for(int i =9; i>=1; i--){
        inv_shift_row(state);
        D{ printf("Round:%d inv shift row state: \n", i); print_arr(state, 16); }
        inv_sub_byte(state);
        D{ printf("Round:%d inv subbyte state: \n", i); print_arr(state, 16); }
        add_roundkey(state, roundKey[i]);
        D{ printf("Round:%d add roundkey state: \n", i); print_arr(state, 16); }
        inv_mix_column(state);
        D{ printf("Round:%d inv mix col state: \n", i); print_arr(state, 16); }
    }
    inv_shift_row(state);
    D{ printf("Round:%d inv shift row state: \n", 10); print_arr(state, 16); }
    inv_sub_byte(state);
    D{ printf("Round:%d inv subbyte state: \n", 10); print_arr(state, 16); }
    add_roundkey(state, roundKey[0]);
    D{ printf("Round:%d add roundkey state: \n", 10); print_arr(state, 16); }
    for(int i = 0; i< 16; i++){
        plaintext[i] = state[i];
    }
}