# c_crypto
C Crypto library 
This repository contains a C implementation of crypto functions:

* **Sysmetric-key Encryption/Decryption**: DES, AES128
* **Hash Function**: SHA256
* **Block Cipher Modes of Operation**: ccm-aes128

## DES
DES encrypt
```C
unsigned char pt[8]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};
unsigned char key[8]={0x13,0x34,0x57,0x79,0x9B,0xBC,0xDF,0xF1};
unsigned uint8_t cipher[8]= {0};
des_encrypt(cipher, key, pt);
```
DES decrypt
```C
unsigned char key[8]={0x13,0x34,0x57,0x79,0x9B,0xBC,0xDF,0xF1};
unsigned char cipher[8]={0x85,0xe8,0x13,0x54,0x0f,0x0a,0xb4,0x05};
unsigned uint8_t pt[8]= {0};
des_decrypt(pt, key, cipher);
```

## AES
AES encrypt
```C
uint8_t key[16]={0x54, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6D, 0x79,
                0x20, 0x4B, 0x75, 0x6E, 0x67, 0x20, 0x46, 0x75};
uint8_t m[16] = {0x54, 0x77, 0x6F, 0x20, 0x4F, 0x6E, 0x65 ,0x20, 
                0x4E, 0x69 ,0x6E, 0x65, 0x20, 0x54, 0x77, 0x6F};
uint8_t cipher[16] = {0};
aes_encrypt(cipher, key, m);
```

AES decrypt
```C
uint8_t key[16]={0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
                0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
uint8_t m[16]={0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
                0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
uint8_t m[16] = {0};
aes_decrypt(m, key, cipher);
```

SHA256 hash
```C
uint8_t m[64] = {0xf6, 0xbb, 0x5d, 0x59, 0xb0, 0xfa, 0x9d, 0xe0, 
                0x82, 0x8b, 0x11, 0x53, 0x03, 0xbf, 0x94, 0xaa, 
                0x98, 0x73, 0x61, 0xcc, 0xdd, 0xe4, 0x8d, 0x02, 
                0x46, 0xc5, 0xd5, 0xab, 0x06, 0x8f, 0x9a, 0x32, 
                0x2f, 0x19, 0x2a, 0x3e, 0x1b, 0x68, 0x41, 0x28, 
                0x0c, 0xc8, 0xd0, 0xb2, 0x0f, 0x1b, 0xfc, 0xf6, 
                0x26, 0x72, 0x6a, 0x9c, 0xa5, 0xda, 0xba, 0x50, 
                0xdd, 0x79, 0x51, 0x73, 0xf8, 0xd9, 0x5c, 0x11};
uint32_t hash[8]={0};
sha256_hash(hash, m, 512);
```

## CCM-AES128
ccm-aes128 encrypt
```C
uint8_t key[16] = {0x40, 0x41, 0x42, 0x43, 
                        0x44, 0x45, 0x46, 0x47, 
                        0x48, 0x49, 0x4a, 0x4b, 
                        0x4c, 0x4d, 0x4e, 0x4f};
uint8_t p[4] = {0x20, 0x21, 0x22, 0x23};
uint8_t n[7] = {0x10, 0x11, 0x12, 0x13, 
                0x14, 0x15, 0x16};
uint8_t a[8] = {0x00, 0x01, 0x02, 0x03, 
                0x04, 0x05, 0x06, 0x07}; 

uint8_t* cipher = malloc(8 * sizeof(uint8_t*));
ccm_aes_encrypt(cipher, p, key, n, a, 7, 8, 4*sizeof(uint8_t), 4);
```

ccm-aes128 decrypt
```C
uint8_t cipher[8] = {0x71, 0x62, 0x01, 0x5b, 0x4d, 0xac, 0x25, 0x5d};
uint8_t n[7] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16};
uint8_t a[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07}; 
uint8_t key[16] = {0x40, 0x41, 0x42, 0x43, 
                        0x44, 0x45, 0x46, 0x47, 
                        0x48, 0x49, 0x4a, 0x4b, 
                        0x4c, 0x4d, 0x4e, 0x4f};
uint8_t* p = malloc(4 * sizeof(uint8_t*));
ccm_aes_decrypt(p, cipher, key, n, a, 7, 8, 8, 4);
```
## ECC
NIST P-256 curve point multiplication
```C
uint16_t px[32] ={0x96, 0xc2, 0x98, 0xd8, 0x45, 0x39, 0xa1, 0xf4,
                0xa0, 0x33, 0xeb, 0x2d, 0x81, 0x7d, 0x03, 0x77,
                0xf2, 0x40, 0xa4, 0x63, 0xe5, 0xe6, 0xbc, 0xf8,
                0x47, 0x42, 0x2c, 0xe1, 0xf2, 0xd1, 0x17, 0x6b};

uint16_t py[32] ={0xf5, 0x51, 0xbf, 0x37, 0x68, 0x40, 0xb6, 0xcb,
                0xce, 0x5e, 0x31, 0x6b, 0x57, 0x33, 0xce, 0x2b,
                0x16, 0x9e, 0x0f, 0x7c, 0x4a, 0xeb, 0xe7, 0x8e,
                0x9b, 0x7f, 0x1a, 0xfe, 0xe2, 0x42, 0xe3, 0x4f};
                
uint16_t k[32] = {0x58, 0x2d, 0xb4, 0xba, 0xae, 0x98, 0xf5, 0xba,
                    0xdb, 0x2c, 0xbd, 0x6c, 0xeb, 0x57, 0xb7, 0x04,
                    0x97, 0xdf, 0x67, 0xc5, 0xef, 0x28, 0x6c, 0xcd,
                    0x9f, 0xb2, 0x71, 0x54, 0x07, 0x91, 0x39, 0xa1};
                    
struct epoint* q = epoint_init();
struct epoint* p = epoint_init();
for (int i=0; i<WORD_LENGTH; i++){
    p->x[i] = px[i];
    p->y[i] = py[i];
}
p256_scalar_mult(q, k, p);
```
# Reference
DES - Schneier, B., 2007. Applied cryptography: protocols, algorithms, and source code in C. john wiley & sons.

AES - https://www.nist.gov/publications/advanced-encryption-standard-aes

SHA256 - https://csrc.nist.gov/projects/hash-functions

CCMAES - https://csrc.nist.gov/publications/detail/sp/800-38c/final

ECC - Hankerson, D., Menezes, A.J. and Vanstone, S., 2006. Guide to elliptic curve cryptography. Springer Science & Business Media.

Arithmetric - Menezes, A.J., Van Oorschot, P.C. and Vanstone, S.A., 2018. Handbook of applied cryptography. CRC press.
