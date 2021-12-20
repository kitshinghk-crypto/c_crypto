#ifndef DES_H
#define DES_H
#include <stddef.h>
typedef int bool;
#define true 1
#define false 0

extern bool DES_DEBUG;
extern int des_encrypt(unsigned char*, const unsigned char*, const unsigned char*);
extern int des_decrypt(unsigned char*, const unsigned char*, const unsigned char*);

//For debug
//extern int ip(unsigned char*); 
//extern int fp(unsigned char*); 
//extern int key_permutation(unsigned char *, const unsigned char *);
//extern int key_rotate_1(unsigned char * );
//extern int key_rotate_2(unsigned char * );
//extern int key_compress(unsigned char * , unsigned char* );
//extern int expansion(unsigned char * , unsigned char* );
//extern unsigned int sbox(unsigned int , unsigned char );
//extern int pbox(unsigned char* );
//extern int sbox_m(unsigned char* );
//extern void printBits(size_t const , void const * const );
#endif