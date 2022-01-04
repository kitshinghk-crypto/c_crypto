#include "arithmetic.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern uint8_t WORD_LENGTH;
extern bool ARITH_DEBUG;

void testadd(){
    WORD_LENGTH = 8;
    uint16_t* a = malloc(WORD_LENGTH * sizeof(uint16_t));
    uint16_t* b = malloc(WORD_LENGTH * sizeof(uint16_t));
    uint8_t carry =0;
    a[0] = 1;
    b[0] = 1;

    printf("Test Add:\n");
    printf("a:\n");
    print_bin(a, WORD_LENGTH);
    printf("b:\n");
    print_bin(b, WORD_LENGTH);
    add(a,b,&carry);
    printf("a+b:\n");
    print_bin(a, WORD_LENGTH);
    printf("carry:\n");
    printf("%u\n", carry);

    a[0] = 255;
    b[0] = 255;
    printf("Test Add:\n");
    printf("a:\n");
    print_bin(a, WORD_LENGTH);
    printf("b:\n");
    print_bin(b, WORD_LENGTH);
    add(a,b,&carry);
    printf("a+b:\n");
    print_bin(a, WORD_LENGTH);
    printf("carry:\n");
    printf("%u\n", carry);
}

void testminus(){
    WORD_LENGTH = 8;
    uint16_t* a = malloc(WORD_LENGTH * sizeof(uint16_t));
    uint16_t* b = malloc(WORD_LENGTH * sizeof(uint16_t));
    uint8_t carry =0;
    a[0] = 3;
    b[0] = 1;
    printf("Test sub:\n");
    printf("a:\n");
    print_bin(a, WORD_LENGTH);
    printf("b:\n");
    print_bin(b, WORD_LENGTH);
    sub(a,b,&carry);
    printf("a-b:\n");
    print_bin(a, WORD_LENGTH);
    printf("carry:\n");
    printf("%u\n", carry);

    a[0] = 255;
    a[1] = 255;
    b[0] = 255;
    printf("Test sub:\n");
    printf("a:\n");
    print_bin(a, WORD_LENGTH);
    printf("b:\n");
    print_bin(b, WORD_LENGTH);
    sub(a,b,&carry);
    printf("a-b:\n");
    print_bin(a, WORD_LENGTH);
    printf("carry:\n");
    printf("%u\n", carry);

    a[0] = 0;
    a[1] = 0;
    b[0] = 1;
    printf("Test sub:\n");
    printf("a:\n");
    print_bin(a, WORD_LENGTH);
    printf("b:\n");
    print_bin(b, WORD_LENGTH);
    sub(a,b,&carry);
    printf("a-b:\n");
    print_bin(a, WORD_LENGTH);
    printf("carry:\n");
    printf("%u\n", carry);

    uint16_t* z = malloc(WORD_LENGTH * sizeof(uint16_t));
    for(uint8_t i = 0; i< WORD_LENGTH; i++){
        z[i] = 0;
    }
    sub(z,a,&carry);
    printf("z- (-1):\n");
    print_bin(z, WORD_LENGTH);
    printf("carry:\n");
    printf("%u\n", carry);
}

void testsignsub(){
    WORD_LENGTH = 8;
    uint16_t* a = malloc(WORD_LENGTH * sizeof(uint16_t));
    uint16_t* b = malloc(WORD_LENGTH * sizeof(uint16_t));
    uint8_t carry =0;
    a[0] = 3;
    b[0] = 1;
    uint8_t a_neg =0;
    uint8_t b_neg =0;

    printf("Test sign sub:\n");
    printf("3-1:\n");
    sign_sub(a,b,&a_neg,&b_neg);
    printf("sign=%u\n", a_neg);
    print_bin(a, WORD_LENGTH);

    set_zero(b); set_zero(a);
    a[0] = 1;b[0] = 3; a_neg =0; b_neg=0;
    printf("1-3:\n");
    sign_sub(a,b,&a_neg,&b_neg);
    printf("sign=%u\n", a_neg);
    print_bin(a, WORD_LENGTH);

    set_zero(b); set_zero(a);
    a[0] = 3;b[0] = 1; a_neg =1; b_neg=0;
    printf("-3-1:\n");
    sign_sub(a,b,&a_neg,&b_neg);
    printf("sign=%u\n", a_neg);
    print_bin(a, WORD_LENGTH);

    set_zero(b); set_zero(a);
    a[0] = 3;b[0] = 1; a_neg =0; b_neg=1;
    printf("3-(-1):\n");
    sign_sub(a,b,&a_neg,&b_neg);
    printf("sign=%u\n", a_neg);
    print_bin(a, WORD_LENGTH);

    set_zero(b); set_zero(a);
    a[0] = 3;b[0] = 1; a_neg =1; b_neg=1;
    printf("-3-(-1):\n");
    sign_sub(a,b,&a_neg,&b_neg);
    printf("sign=%u\n", a_neg);
    print_bin(a, WORD_LENGTH);

    set_zero(b); set_zero(a);
    a[0] = 3;b[0] = 7; a_neg =1; b_neg=1;
    printf("-3-(-7):\n");
    sign_sub(a,b,&a_neg,&b_neg);
    printf("sign=%u\n", a_neg);
    print_bin(a, WORD_LENGTH);
}

void testsignadd(){
    WORD_LENGTH = 8;
    uint16_t* a = malloc(WORD_LENGTH * sizeof(uint16_t));
    uint16_t* b = malloc(WORD_LENGTH * sizeof(uint16_t));
    uint8_t carry =0;
    a[0] = 3;
    b[0] = 1;
    uint8_t a_neg =0;
    uint8_t b_neg =0;

    printf("Test sign add:\n");
    printf("3+1:\n");
    sign_add(a,b,&a_neg,&b_neg);
    printf("sign=%u\n", a_neg);
    print_bin(a, WORD_LENGTH);

    set_zero(b); set_zero(a);
    a[0] = 3;b[0] = 1; a_neg =1; b_neg=0;
    printf("-3+1:\n");
    sign_add(a,b,&a_neg,&b_neg);
    printf("sign=%u\n", a_neg);
    print_bin(a, WORD_LENGTH);

    set_zero(b); set_zero(a);
    a[0] = 3;b[0] = 7; a_neg =1; b_neg=0;
    printf("-3+7:\n");
    sign_add(a,b,&a_neg,&b_neg);
    printf("sign=%u\n", a_neg);
    print_bin(a, WORD_LENGTH);

    set_zero(b); set_zero(a);
    a[0] = 3;b[0] = 1; a_neg =0; b_neg=1;
    printf("3+(-1):\n");
    sign_add(a,b,&a_neg,&b_neg);
    printf("sign=%u\n", a_neg);
    print_bin(a, WORD_LENGTH);

    set_zero(b); set_zero(a);
    a[0] = 3;b[0] = 8; a_neg =0; b_neg=1;
    printf("3+(-8):\n");
    sign_add(a,b,&a_neg,&b_neg);
    printf("sign=%u\n", a_neg);
    print_bin(a, WORD_LENGTH);

    set_zero(b); set_zero(a);
    a[0] = 3;b[0] = 1; a_neg =1; b_neg=1;
    printf("-3+(-1):\n");
    sign_add(a,b,&a_neg,&b_neg);
    printf("sign=%u\n", a_neg);
    print_bin(a, WORD_LENGTH);

    set_zero(b); set_zero(a);
    a[0] = 3;b[0] = 7; a_neg =1; b_neg=1;
    printf("-3+(-7):\n");
    sign_add(a,b,&a_neg,&b_neg);
    printf("sign=%u\n", a_neg);
    print_bin(a, WORD_LENGTH);
}

void testneg(){
    WORD_LENGTH = 8;
    uint16_t* a = malloc(WORD_LENGTH * sizeof(uint16_t));
    uint16_t* b = malloc(WORD_LENGTH * sizeof(uint16_t));
    uint8_t carry =0;
    a[0] = 0;
    a[1] = 0;
    b[0] = 1;
    b[1] = 255;
    printf("Test neg:\n");
    print_bin(a, WORD_LENGTH);
    print_bin(b, WORD_LENGTH);
    sub(a,b,&carry);
    printf("before neg:\n");
    print_bin(a, WORD_LENGTH);
    neg(a);
    printf("after neg:\n");
    print_bin(a, WORD_LENGTH);
}


void testmodadd(){
    WORD_LENGTH = 8;
    uint16_t* a = malloc(WORD_LENGTH * sizeof(uint16_t));
    uint16_t* b = malloc(WORD_LENGTH * sizeof(uint16_t));
    uint16_t* p = malloc(WORD_LENGTH * sizeof(uint16_t));
    uint8_t carry =0;
    a[0] = 3;
    b[0] = 9;
    p[0] = 11;

    printf("Test Mod Add:\n");
    printf("a:\n");
    print_bin(a, WORD_LENGTH);
    printf("b:\n");
    print_bin(b, WORD_LENGTH);
    mod_add(a,b,p);
    printf("a+b:\n");
    print_bin(a, WORD_LENGTH);
    printf("carry:\n");
    printf("%u\n", carry);
}


void testmodsub(){
    WORD_LENGTH = 8;
    uint16_t* a = malloc(WORD_LENGTH * sizeof(uint16_t));
    uint16_t* b = malloc(WORD_LENGTH * sizeof(uint16_t));
    uint16_t* p = malloc(WORD_LENGTH * sizeof(uint16_t));
    uint8_t carry =0;
    a[0] = 3;
    b[0] = 9;
    p[0] = 11;

    printf("Test Mod Sub:\n");
    printf("a:\n");
    print_bin(a, WORD_LENGTH);
    printf("b:\n");
    print_bin(b, WORD_LENGTH);
    mod_sub(a,b,p);
    printf("a-b:\n");
    print_bin(a, WORD_LENGTH);
    printf("carry:\n");
    printf("%u\n", carry);

    carry = 0;
    a[0] = 9;
    b[0] = 3;
    p[0] = 11;

    printf("Test Mod Sub:\n");
    printf("a:\n");
    print_bin(a, WORD_LENGTH);
    printf("b:\n");
    print_bin(b, WORD_LENGTH);
    mod_sub(a,b,p);
    printf("a-b:\n");
    print_bin(a, WORD_LENGTH);
    printf("carry:\n");
    printf("%u\n", carry);
}


void testmult(){
    WORD_LENGTH = 8;
    uint16_t* a = malloc(WORD_LENGTH * sizeof(uint16_t));
    uint16_t* b = malloc(WORD_LENGTH * sizeof(uint16_t));
    uint16_t* c = malloc(WORD_LENGTH * 2 * sizeof(uint16_t));
    a[0] = 3;
    b[0] = 9;

    printf("Test Mod Sub:\n");
    printf("a:\n");
    print_bin(a, WORD_LENGTH);
    printf("b:\n");
    print_bin(b, WORD_LENGTH);
    mult(c,a,b);
    printf("a*b:\n");
    print_bin(c, WORD_LENGTH * 2);

    a[0] = 255;
    b[0] = 255;

    printf("Test Mod mult:\n");
    printf("a:\n");
    print_bin(a, WORD_LENGTH);
    printf("b:\n");
    print_bin(b, WORD_LENGTH);
    mult(c,a,b);
    printf("a*b:\n");
    print_bin(c, WORD_LENGTH * 2);
}

void testdivide(){
    ARITH_DEBUG = false;

    uint16_t* a = malloc(1 * sizeof(uint16_t));
    uint16_t* b = malloc(1 * sizeof(uint16_t));
    a[0] = 255;
    b[0] = 7;
    uint16_t * q = malloc((1) * sizeof(uint16_t));
    uint16_t * r = malloc((1) * sizeof(uint16_t));
    
    printf("Test divide:\n");
    divide(q,r,a,b,0,0);
    printf("q:\n");
    print_hex(q,1);
    printf("r:\n");
    print_hex(r,1);
    free(q);
    free(r);
    q=NULL; r=NULL;
    puts("");

    uint16_t *q2 = malloc((2) * sizeof(uint16_t));
    uint16_t *r2 = malloc((1) * sizeof(uint16_t));
    a[0] = 10;
    a[1] = 255;
    b[0] = 7;
    printf("Test divide:\n");
    divide(q2,r2,a,b,1,0);
    printf("q:\n");
    print_hex(q2,2);
    printf("r:\n");
    print_hex(r2,1);
    free(q2);
    free(r2);
    q2=NULL; r2=NULL;
    puts("");

    uint16_t *q3 = malloc((2) * sizeof(uint16_t));  
    uint16_t *r3 = malloc((3) * sizeof(uint16_t));
    puts("");
    a[0] = 0xa7;
    a[1] = 0x0e;
    a[2] = 0x08;
    a[3] = 0x2b;
    b[0] = 0xed;
    b[1] = 0x49;
    b[2] = 0x01;
    printf("Test divide:\n");
    divide(q3,r3,a,b,3,2);
    printf("q:\n");
    print_hex(q3,2);
    printf("r:\n");
    print_hex(r3,3);
    free(q3);
    free(r3);
    q3=NULL; r3=NULL;
}

void testhalf(){
    WORD_LENGTH = 8;
    uint16_t* a = malloc(WORD_LENGTH * sizeof(uint16_t));
    for(size_t i =0; i< WORD_LENGTH;i++){
        a[i] =0xaa;
    }
    printf("Test half:\n");
    printf("Before a:\n");
    print_bin(a, WORD_LENGTH);
    half(a);
    printf("After a:\n");
    print_bin(a, WORD_LENGTH);
}

void testtimes_two(){
    WORD_LENGTH = 8;
    uint16_t* a = malloc(WORD_LENGTH * sizeof(uint16_t));
    for(size_t i =0; i< WORD_LENGTH;i++){
        a[i] =0;
    }
    a[0] = 255;
    printf("Test times two:\n");
    printf("Before a:\n");
    print_bin(a, WORD_LENGTH);
    times_two(a);
    printf("After a:\n");
    print_bin(a, WORD_LENGTH);
}

void testinverse(){
    ARITH_DEBUG = false;
    WORD_LENGTH = 8;
    uint16_t* inv = malloc(WORD_LENGTH * sizeof(uint16_t));
    uint16_t* x = malloc(WORD_LENGTH * sizeof(uint16_t));
    uint16_t* y = malloc(WORD_LENGTH * sizeof(uint16_t));
    uint16_t* tmp = malloc(WORD_LENGTH * sizeof(uint16_t));
    for(uint8_t i =0; i<WORD_LENGTH; i++){
        inv[i] =0; x[i]=0; y[i] =0; tmp[i] =0;
    }
    x[0] = 0xb5; x[1]=0x02;
    y[0] = 0x61; y[1]=0x02;
    printf("Test inverse:\n");
    inverse(inv, x, y);
    printf("x:\n");
    print_dec(x, WORD_LENGTH);
    printf("y:\n");
    print_dec(y, WORD_LENGTH);
    printf("inv:\n");
    print_dec(inv, WORD_LENGTH);

    set_zero(x); set_zero(y);set_zero(inv);
    x[0] = 0x7f; x[1]=0x01;
    y[0] = 0x0f; y[1]=0x01;
    printf("Test inverse:\n");
    inverse(inv, x, y);
    printf("x:\n");
    print_dec(x, WORD_LENGTH);
    printf("y:\n");
    print_dec(y, WORD_LENGTH);
    printf("inv:\n");
    print_dec(inv, WORD_LENGTH);
}

int main(){
    testadd(); 
    testminus();
    testneg();
    testsignsub();
    testsignadd();
    //testmodadd();
    //testmodsub();
    //testmult();
    testdivide();
    //testhalf();
    //testtimes_two();
    //testinverse();
    return 1;
}