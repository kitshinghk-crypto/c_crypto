#include "des.h"
#include <stdio.h>

void testIP(){
    unsigned char pt[8]={1};
    printBits(sizeof(pt)/sizeof(unsigned char), pt);
    ip(pt);
    printBits(sizeof(pt)/sizeof(unsigned char), pt);
}

void testFP(){
    unsigned char pt[8]={1};
    printBits(sizeof(pt)/sizeof(unsigned char), pt);
    fp(pt);
    printBits(sizeof(pt)/sizeof(unsigned char), pt);
}

void testKP(){
    unsigned char k[8]={0x00, 0x00,0x00,0x00, 0xff,0xff,0xff,0xff};
    unsigned char pk[7];
    printBits(sizeof(k)/sizeof(unsigned char), k);
    key_permutation(pk, k);
    printBits(sizeof(pk)/sizeof(unsigned char), pk);
}

void testRotate(){
    unsigned char k[7]={1};
    printBits(sizeof(k)/sizeof(unsigned char), k);
    key_rotate_1(k);
    printBits(sizeof(k)/sizeof(unsigned char), k);

    unsigned char k2[7]={1};
    key_rotate_2(k2);
    printBits(sizeof(k2)/sizeof(unsigned char), k2);
}

void testCK(){
    unsigned char k[7]={1};
    unsigned char ck[6];
    printBits(sizeof(k)/sizeof(unsigned char), k);
    key_compress(ck, k);
    printBits(sizeof(ck)/sizeof(unsigned char), ck);
}

void testEXP(){
    unsigned char m[4]={8};
    unsigned char em[6];
    printBits(sizeof(m)/sizeof(unsigned char), m);
    expansion(em, m);
    printBits(sizeof(em)/sizeof(unsigned char), em);
}

void testSbox(){
    printf("sbox=%u, value=%u\n", 1U, sbox(1, 0U));
    printf("sbox=%u, value=%u\n", 1U, sbox(1,17U));
    printf("sbox=%u, value=%u\n", 1U, sbox(1,63U));

    printf("sbox=%u, value=%u\n", 2U, sbox(2,0U));
    printf("sbox=%u, value=%u\n", 2U, sbox(2,17U));
    printf("sbox=%u, value=%u\n", 2U, sbox(2,63U));

    printf("sbox=%u, value=%u\n", 3U, sbox(3,0U));
    printf("sbox=%u, value=%u\n", 3U, sbox(3,17U));
    printf("sbox=%u, value=%u\n", 3U, sbox(3,63U));

    printf("sbox=%u, value=%u\n", 4U, sbox(4,0U));
    printf("sbox=%u, value=%u\n", 4U, sbox(4,17U));
    printf("sbox=%u, value=%u\n", 4U, sbox(4,63U));

    printf("sbox=%u, value=%u\n", 5U, sbox(5,0U));
    printf("sbox=%u, value=%u\n", 5U, sbox(5,17U));
    printf("sbox=%u, value=%u\n", 5U, sbox(5,63U));

    printf("sbox=%u, value=%u\n", 6U, sbox(6,0U));
    printf("sbox=%u, value=%u\n", 6U, sbox(6,17U));
    printf("sbox=%u, value=%u\n", 6U, sbox(6,63U));

    printf("sbox=%u, value=%u\n", 7U, sbox(7,0U));
    printf("sbox=%u, value=%u\n", 7U, sbox(7,17U));
    printf("sbox=%u, value=%u\n", 7U, sbox(7,63U));

    printf("sbox=%u, value=%u\n", 8U, sbox(8,0U));
    printf("sbox=%u, value=%u\n", 8U, sbox(8,17U));
    printf("sbox=%u, value=%u\n", 8U, sbox(8,63U));
}

void testpbox(){
    unsigned char m[4]={1};
    printBits(sizeof(m)/sizeof(unsigned char), m);
    pbox(m);
    printBits(sizeof(m)/sizeof(unsigned char), m);
}

void testsbox2(){
    unsigned char em[6]={1};
    printBits(sizeof(em)/sizeof(unsigned char), em);
    sbox_m(em);
    printBits(sizeof(em)/sizeof(unsigned char), em);
}

void testdesecrypt(){
    //01 23 45 67 89 AB CD EF
    unsigned char pt[8]={0xef,0xcd,0xab,0x89,0x67,0x45,0x23,0x01};
    unsigned char k[8]={0xf1,0xdf,0xbc,0x9b,0x79,0x57,0x34,0x13};
    //unsigned char k[8]={0x12,0x34,0x56,0x78,0x9a,0xbc,0xde,0xF0};
    unsigned char cipher[8];
    puts("plaintext: ");
    for(int i=7; i>=0; i--){
        printf("%x ", pt[i]);
    }
    puts("");
    puts("key: ");
    for(int i=7; i>=0; i--){
        printf("%x ", k[i]);
    }
    puts("");
    des_encrypt(cipher, k, pt);
    puts("chipher: ");
    for(int i=7; i>=0; i--){
        printf("%x ", cipher[i]);
    }
    puts("");
}

int main(void){
    //testIP();
    testKP();
    //testRotate();
    //testCK();
    //testEXP();
    //testSbox();
    //testpbox();
    //testFP();
    //testsbox2();
    testdesecrypt();
}