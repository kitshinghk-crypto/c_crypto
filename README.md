# c_crypto
C Crypto library 
This repository contains a C implementation of crypto functions:

* **Sysmetric-key Encryption/Decryption**: DES, AES128
* **Hash Function**: SHA256
* **Block Cipher Modes of Operation**: ccm-aes128
* **ECC**: NIST P-256 curve point multiplication

## DES
### DES Encryption:
```C
void des_encrypt(uint8_t* cipher, const uint8_t* key, const uint8_t* plaintext*);
```
**Parameters:**

*cipher* - A uint8_t pointer to the ouput cipher.

*key* - A uint8_t pointer to the secret key.

*plaintext* - a uint8_t pointer to the plaintext to be encrypted.

**Example:**
```C
uint8_t pt[8]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};
uint8_t key[8]={0x13,0x34,0x57,0x79,0x9B,0xBC,0xDF,0xF1};
uint8_t cipher[8]= {0};
des_encrypt(cipher, key, pt);
```
### DES decryption
```C
void des_decrypt(uint8_t* plaintext, const uint8_t* key, const uint8_t* cipher);
```
**Parameters:**

*plaintext* - A uint8_t pointer to the ouput decrypted plaintext.

*key* - A uint8_t pointer to the secret key.

*cipher* - a uint8_t pointer to the cipher to be decrypted.

**Example:**
```C
uint8_t key[8]={0x13,0x34,0x57,0x79,0x9B,0xBC,0xDF,0xF1};
uint8_t cipher[8]={0x85,0xe8,0x13,0x54,0x0f,0x0a,0xb4,0x05};
uint8_t pt[8]= {0};
des_decrypt(pt, key, cipher);
```

## AES
### AES encryption
```C
void aes_encrypt(uint8_t* cipher, const uint8_t* key, const uint8_t* plaintext);
```
**Parameters:**

*cipher* - A uint8_t pointer to the ouput cipher.

*key* - A uint8_t pointer to the secret key.

*plaintext* - a uint8_t pointer to the plaintext to be encrypted.

**Example:**
```C
uint8_t key[16]={0x54, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6D, 0x79,
                0x20, 0x4B, 0x75, 0x6E, 0x67, 0x20, 0x46, 0x75};
uint8_t m[16] = {0x54, 0x77, 0x6F, 0x20, 0x4F, 0x6E, 0x65 ,0x20, 
                0x4E, 0x69 ,0x6E, 0x65, 0x20, 0x54, 0x77, 0x6F};
uint8_t cipher[16] = {0};
aes_encrypt(cipher, key, m);
```

### AES decryption
```C
void aes_decrypt(uint8_t* plaintext, const uint8_t* key, const uint8_t* cipher);
```

**Parameters:**

*plaintext* - A uint8_t pointer to the ouput decrypted plaintext.

*key* - A uint8_t pointer to the secret key.

*cipher* - a uint8_t pointer to the cipher to be decrypted.

**Example:**
```C
uint8_t key[16]={0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
                0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
uint8_t m[16]={0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
                0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
uint8_t m[16] = {0};
aes_decrypt(m, key, cipher);
```

## SHA256 
```C
int sha256_hash(uint32_t* hash, uint8_t* m, size_t len );
```
**Parameters:**

*hash* - a uint32_t pointer to the output hash.

*m* - a uint8_t pointer to the message.

*len* - the bit-length of the message.

**Example:**
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
### ccm-aes128 Encryption
```C
void ccm_aes_encrypt(uint8_t* cipher, const uint8_t* plaintext, const uint8_t* key, const uint8_t* nonce,
                            const uint8_t* a, uint8_t nlen, size_t alen, size_t plen, uint8_t tlen);
```
**Parameters:**

*cipher* - a uint8_t pointer to the output cipher.

*plaintext* - a uint8_t pointer to the plaintext to be encrypted.

*key* - a uint8_t pointer to the secret key.

*nonce* - a uint8_t pointer to the nonce.

*a* - a uint8_t pointer to the associated data.

*nlen* - The bit-length of nonce.

*alen* - The bit-length of associated data.

*plen* - The bit-length of the plaintext.

*tlen* - the bit-length of the tag.

**Example:**
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

uint8_t cipher[8] = {0};
ccm_aes_encrypt(cipher, p, key, n, a, 7, 8, 4*sizeof(uint8_t), 4);
```

ccm-aes128 Decryption
```C
int ccm_aes_decrypt(uint8_t* plaintext, const uint8_t* cipher, const uint8_t* key, const uint8_t* nonce,
                            const uint8_t* a, uint8_t nlen, size_t alen, size_t clen, uint8_t tlen);
```
**Parameters:**

*plaintext* - a uint8_t pointer to the output decryptedplaintext.

*cipher* - a uint8_t pointer to the input cipher.

*key* - a uint8_t pointer to the secret key.

*nonce* - a uint8_t pointer to the nonce.

*a* - a uint8_t pointer to the associated data.

*nlen* - The bit-length of nonce.

*alen* - The bit-length of associated data.

*plen* - The bit-length of the plaintext.

*tlen* - the bit-length of the tag

**Example:**
```C
uint8_t cipher[8] = {0x71, 0x62, 0x01, 0x5b, 0x4d, 0xac, 0x25, 0x5d};
uint8_t n[7] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16};
uint8_t a[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07}; 
uint8_t key[16] = {0x40, 0x41, 0x42, 0x43, 
                        0x44, 0x45, 0x46, 0x47, 
                        0x48, 0x49, 0x4a, 0x4b, 
                        0x4c, 0x4d, 0x4e, 0x4f};
uint8_t p[4] = {0};
ccm_aes_decrypt(p, cipher, key, n, a, 7, 8, 8, 4);
```
## ECC
### NIST P-256 curve point multiplication
```C
void p256_scalar_mult(struct epoint* kp, const uint8_t* k, const struct epoint* p)
```
**Parameters:**

*kp* - A pointer to scalar elliptic point output 

*k* - A uint8_t pointer to the scalar

*p* - a pointer to a elliptic point

**Example:**
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

# ECDSA
## ECDSA signature generation
```C
void ecdsa_p256_sha256_sign(uint8_t* r, uint8_t* s, const uint8_t* d, const uint8_t* k, const uint8_t* msg, size_t mlen);
```
**Parameters:**

*r* - a uint8_t pointer to the output signature, R.

*s* - a uint8_t pointer to the output signature, S.

*d* - a uint8_t pointer to the private key.

*k* - a uint8_t pointer to a random value, k.

*msg* - a uint8_t pointer to the message.

*mlen* - the bit-length of the message.

**Example:**
```c
uint8_t msg[128] ={0xe8, 0x9c, 0xd7, 0x97, 0xbd, 0xde, 0xd0, 0xb0, 0xc9, 0xd5, 0x46, 0x61, 0xd1, 0x08, 0xd3, 0xe2, 0x0f, 0x79, 0x99, 0x37, 0xd0, 0x54, 0x37, 0xeb, 0x91, 0x14, 0xe9, 0x91, 0xd7, 0xa9, 0x34, 0xc9, 0x7c, 0xc1, 0x65, 0x7a, 0xea, 0x71, 0xe4, 0xa5, 0x86, 0xd3, 0x9b, 0xbf, 0x75, 0x81, 0xd2, 0x8a, 0x8a, 0x85, 0xb5, 0x3c, 0xb5, 0x65, 0x80, 0xd2, 0xdf, 0x1d, 0xfb, 0x05, 0x36, 0x25, 0x3e, 0x47, 0xb5, 0xfe, 0xd3, 0x72, 0xa6, 0x0c, 0x96, 0x37, 0x9a, 0x1f, 0x46, 0xa7, 0x10, 0x43, 0x6c, 0xf4, 0xbd, 0xa3, 0xdc, 0xe6, 0xd2, 0xbc, 0xe2, 0x5e, 0x04, 0x1a, 0x5b, 0x16, 0xfe, 0x83, 0x69, 0x41, 0xaf, 0xfa, 0x35, 0x29, 0x48, 0x25, 0xbd, 0x0c, 0xec, 0xdc, 0x42, 0xc9, 0x5f, 0xad, 0xcc, 0xe2, 0xd9, 0xf2, 0xa6, 0x3d, 0xee, 0x43, 0x3e, 0xf7, 0x21, 0x74, 0xc7, 0x77, 0x88, 0x23, 0x05, 0x59};
size_t mlen = 1024;
uint8_t d[32] = {0x64, 0xb4, 0x72, 0xda, 0x6d, 0xa5, 0x54, 0xca, 0xac, 0x3e, 0x4e, 0x0b, 0x13, 0xc8, 0x44, 0x5b, 0x1a, 0x77, 0xf4, 0x59, 0xee, 0xa8, 0x4f, 0x1f, 0x58, 0x8b, 0x5f, 0x71, 0x3d, 0x42, 0x9b, 0x51};
uint8_t k[32] = {0xde, 0x68, 0x2a, 0x64, 0x87, 0x07, 0x67, 0xb9, 0x33, 0x5d, 0x4f, 0x82, 0x47, 0x62, 0x4a, 0x3b, 0x7f, 0x3c, 0xe9, 0xf9, 0x45, 0xf2, 0x80, 0xa2, 0x61, 0x6a, 0x90, 0x4b, 0xb1, 0xbb, 0xa1, 0x94};
uint8_t s[32]={0};
uint8_t r[32]={0};

ecdsa_p256_sha256_sign(r, s, d, k, msg, mlen);
```

## ECDSA signature verification
```C
bool ecdsa_p256_sha256_verify(const uint8_t* r, const uint8_t* s, const uint8_t* qx, const uint8_t* qy,  const uint8_t* msg, size_t mlen);
```
**Parameters:**

*r* - a uint8_t pointer to the signature, R.

*s* - a uint8_t pointer to the signature, S.

*qx* - a uint8_t pointer to the x-coordinate of the public key of the signature signer.

*qy* - a uint8_t pointer to the y-coordinate of the public key of the signature signer.

*msg* - a uint8_t pointer to the message.

*mlen* - the bit-length of the message.

**Return value:**

Return int, 1 = valid signature, 0 = invalid signature.

```c
uint8_t msg[128] ={0xe8, 0x9c, 0xd7, 0x97, 0xbd, 0xde, 0xd0, 0xb0, 0xc9, 0xd5, 0x46, 0x61, 0xd1, 0x08, 0xd3, 0xe2, 0x0f, 0x79, 0x99, 0x37, 0xd0, 0x54, 0x37, 0xeb, 0x91, 0x14, 0xe9, 0x91, 0xd7, 0xa9, 0x34, 0xc9, 0x7c, 0xc1, 0x65, 0x7a, 0xea, 0x71, 0xe4, 0xa5, 0x86, 0xd3, 0x9b, 0xbf, 0x75, 0x81, 0xd2, 0x8a, 0x8a, 0x85, 0xb5, 0x3c, 0xb5, 0x65, 0x80, 0xd2, 0xdf, 0x1d, 0xfb, 0x05, 0x36, 0x25, 0x3e, 0x47, 0xb5, 0xfe, 0xd3, 0x72, 0xa6, 0x0c, 0x96, 0x37, 0x9a, 0x1f, 0x46, 0xa7, 0x10, 0x43, 0x6c, 0xf4, 0xbd, 0xa3, 0xdc, 0xe6, 0xd2, 0xbc, 0xe2, 0x5e, 0x04, 0x1a, 0x5b, 0x16, 0xfe, 0x83, 0x69, 0x41, 0xaf, 0xfa, 0x35, 0x29, 0x48, 0x25, 0xbd, 0x0c, 0xec, 0xdc, 0x42, 0xc9, 0x5f, 0xad, 0xcc, 0xe2, 0xd9, 0xf2, 0xa6, 0x3d, 0xee, 0x43, 0x3e, 0xf7, 0x21, 0x74, 0xc7, 0x77, 0x88, 0x23, 0x05, 0x59};
size_t mlen = 1024;
uint8_t qx[32] ={0x83,0xbf,0x71,0xc2,0x46,0xff,0x59,0x3c,0x2f,0xb1,0xbf,0x4b,0xe9,0x5d,0x56,0xd3,0xcc,0x8f,0xdb,0x48,0xa2,0xbf,0x33,0xf0,0xf4,0xc7,0x5f,0x07,0x1c,0xe9,0xcb,0x1c};
uint8_t qy[32] ={0xa9,0x4c,0x9a,0xa8,0x5c,0xcd,0x7c,0xdc,0x78,0x4e,0x40,0xb7,0x93,0xca,0xb7,0x6d,0xe0,0x13,0x61,0x0e,0x2c,0xdb,0x1f,0x1a,0xa2,0xf9,0x11,0x88,0xc6,0x14,0x40,0xce};
uint8_t s[32]={0x03, 0x89, 0x05, 0xcc, 0x2a, 0xda, 0xcd, 0x3c, 0x5a, 0x17, 0x6f, 0xe9, 0x18, 0xb2, 0x97, 0xef, 0x1c, 0x37, 0xf7, 0x2b, 0x26, 0x76, 0x6c, 0x78, 0xb2, 0xa6, 0x05, 0xca, 0x19, 0x78, 0xf7, 0x8b};
uint8_t r[32]={0xac, 0xc2, 0xc8, 0x79, 0x6f, 0x5e, 0xbb, 0xca, 0x7a, 0x5a, 0x55, 0x6a, 0x1f, 0x6b, 0xfd, 0x2a, 0xed, 0x27, 0x95, 0x62, 0xd6, 0xe3, 0x43, 0x88, 0x5b, 0x79, 0x14, 0xb5, 0x61, 0x80, 0xac, 0xf3};
assert(ecdsa_p256_sha256_verify(r, s, qx, qy, msg, mlen));
```

# Reference
DES - Schneier, B., 2007. Applied cryptography: protocols, algorithms, and source code in C. john wiley & sons.

AES - https://www.nist.gov/publications/advanced-encryption-standard-aes

SHA256 - https://csrc.nist.gov/projects/hash-functions

CCMAES - https://csrc.nist.gov/publications/detail/sp/800-38c/final

ECC - Hankerson, D., Menezes, A.J. and Vanstone, S., 2006. Guide to elliptic curve cryptography. Springer Science & Business Media.

Arithmetric - Menezes, A.J., Van Oorschot, P.C. and Vanstone, S.A., 2018. Handbook of applied cryptography. CRC press.
