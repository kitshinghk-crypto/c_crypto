#include "arithmetic.h"
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#define D if(ARITH_DEBUG)

bool ARITH_DEBUG = false;

void print_dec(uint16_t* b, uint8_t len){
    for(int i = len-1; i>=0; --i){
        printf("%d ", b[i]&0x00ff);
    }
    puts("");
}

void print_hex(uint16_t* b, uint8_t len){
    for(int i = len-1; i>=0; --i){
        printf("%x ", b[i]&0x00ff);
    }
    puts("");
}

void print_bin(uint16_t* ptr, uint8_t size){
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (ptr[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
    }
    puts("");
}

/**  Assume word length = 8
**   01 00 00 00 00 00 00 00
**   0  1  2  3  4  5  6  7
**   LSB                  LMB
**/  
uint8_t WORD_LENGTH = 32;

int compare_len(uint16_t *a, uint16_t*b, uint8_t len){
    for(uint8_t i = len-1; i>=0; i--){
        if(a[i]>b[i]){
            return 1;
        }else if(a[i]<b[i]){
            return -1; 
        }
    }
    return 0;
}

// a>b, return 1
// a<b, return -1
// a=b, return 0
int compare(uint16_t *a, uint16_t*b){
    for(int i = WORD_LENGTH-1; i>=0; i--){
        if(a[i]>b[i]){
            return 1;
        }else if(a[i]<b[i]){
            return -1; 
        }
    }
    return 0;
}

int isZero(uint16_t *a){
    for(uint8_t i = 0; i<WORD_LENGTH; i++){
        if((a[i]&0xff) > 0){
            return 0;
        }
    }
    return 1;
}

void set_zero(uint16_t* a){
    for(uint8_t i = 0; i<WORD_LENGTH; i++){
        a[i] = 0;
    }
}

void half_len(uint16_t*a, uint8_t len){
    for(size_t i = 0; i<len*8; ++i){
        uint8_t bit = (a[(i+1)/8] >> (i+1)%8) & 1U;
        a[i/8] = (a[i/8] & ~(1U<<(i%8))) | (bit << (i%8));
    }
    a[len-1] = a[len-1] & 127;
}

void half(uint16_t*a){
    half_len(a, WORD_LENGTH);
}

void times_two_len(uint16_t* a, uint8_t len){
    for(size_t i =len*8-1; i>=1; --i){
        uint8_t bit = (a[(i-1)/8] >> (i-1)%8) & 1U;
        a[i/8] = (a[i/8] & ~(1U<<(i%8))) | (bit << (i%8));
    }
    a[0] = a[0] & 254;
}

void times_two(uint16_t* a){
    times_two_len(a, WORD_LENGTH);
}

//a = a+b
void add(uint16_t* a, uint16_t* b, uint8_t* carry){
    uint16_t c = 0; 
    a[0] = a[0] + b[0];
    c = (a[0]>>8) & 0x0001;
    a[0] = a[0] & 0x00ff;
    for (uint8_t i=1; i < WORD_LENGTH; ++i){
        a[i] = a[i] + b[i] + c;
        c = (a[i]>>8) & 0x00ff;
        a[i] = a[i] & 0x00ff;
    }
    *carry = c;
}

void add_len(uint16_t* a, uint16_t* b, uint8_t* carry, uint8_t len){
    uint16_t c = 0; 
    a[0] = a[0] + b[0];
    c = (a[0]>>8) & 0x0001;
    a[0] = a[0] & 0x00ff;
    for (uint8_t i=1; i < len; ++i){
        a[i] = a[i] + b[i] + c;
        c = (a[i]>>8) & 0x00ff;
        a[i] = a[i] & 0x00ff;
    }
    *carry = c;
}

//c = a*b
void mult(uint16_t*c, uint16_t* a, uint16_t* b){
    for(uint8_t i = 0; i< WORD_LENGTH*2; ++i){
        c[i] = 0;
    }
    for(uint8_t i = 0; i< WORD_LENGTH; ++i){
        uint8_t u = 0;
        uint16_t uv = 0;
        for(uint8_t j = 0; j< WORD_LENGTH; ++j){
            uv = c[i+j] + a[i]*b[j] + u;
            c[i+j] = uv & 0xff;
            u = (uv >> 8)&0xff;
        }
        c[i+WORD_LENGTH] = (uv>>8)&0xff;
    }
}

//a=a-b
void sub(uint16_t* a, uint16_t* b, uint8_t* carry){
    uint16_t c = 0; 
    a[0] = a[0] - b[0];
    c = (a[0]>>8) & 0x0001;
    a[0] = a[0] & 0x00ff;
    for (uint8_t i=1; i < WORD_LENGTH; ++i){
        a[i] = a[i] - b[i] - c;
        c = (a[i]>>8) & 0x0001;
        a[i] = a[i] & 0x00ff;
    }
    *carry = c;
}

void sub_len(uint16_t* a, uint16_t* b, uint8_t* carry, uint8_t len){
    uint16_t c = 0; 
    a[0] = a[0] - b[0];
    c = (a[0]>>8) & 0x0001;
    a[0] = a[0] & 0x00ff;
    for (uint8_t i=1; i < len; ++i){
        a[i] = a[i] - b[i] - c;
        c = (a[i]>>8) & 0x0001;
        a[i] = a[i] & 0x00ff;
    }
    *carry = c;
}


//a= (a+b) mod p
void mod_add(uint16_t *a, uint16_t *b, uint16_t* p){
    uint8_t carry=0;
    add(a,b,&carry);
    if(carry == 1){
        carry = 0;
        sub(a,p,&carry);
    }else{
        if(compare(a,p)==1){
            carry = 0;
            sub(a,p,&carry);
        }
    }
}

//a= (a-b) mod p
void mod_sub(uint16_t *a, uint16_t *b, uint16_t* p){
    uint8_t carry=0;
    sub(a,b,&carry);
    if (carry == 1){
        carry = 0;
        add(a, p, &carry);
    }
}

void neg(uint16_t *x){
    uint16_t * z = malloc(sizeof(uint16_t)* WORD_LENGTH);
    for(uint8_t i =0; i<WORD_LENGTH; i++){
        z[i] = 0;
    }
    uint8_t carry =0;
    sub(z, x, &carry);
    for(uint8_t i =0; i<WORD_LENGTH; i++){
        x[i] = z[i];
    }
}

void sign_sub(uint16_t* x , uint16_t* y, uint8_t* x_neg, uint8_t* y_neg){
    uint8_t carry =0 ;
    if(*x_neg == 0 && *y_neg ==0){
        sub(x,y,&carry);
        if(carry==1){
            neg(x);*x_neg=1;
        }
    }else if(*x_neg ==1 && *y_neg ==0){
        add(x,y,&carry);
    }else if(*x_neg ==0 && *y_neg ==1){
        add(x,y,&carry);
    }else if(*x_neg ==1 && *y_neg ==1){
        sub(x,y,&carry); 
        if(carry ==1){
            neg(x);*x_neg=0;
        }
    }
}

void sign_add(uint16_t* x , uint16_t* y, uint8_t* x_neg, uint8_t* y_neg){
    uint8_t carry =0 ;
    if(*x_neg == 0 && *y_neg ==0){
        add(x,y,&carry);
    }else if(*x_neg ==1 && *y_neg ==0){
        sub(x,y,&carry);
        if(carry==1){
            neg(x);*x_neg=0;
        }
    }else if(*x_neg ==0 && *y_neg ==1){
        sub(x,y,&carry); 
        if(carry ==1){
            neg(x);*x_neg=1;
        }
    }else if(*x_neg ==1 && *y_neg ==1){
        add(x,y,&carry);
    }
}

/**
**  Ref: Handbook of Applied Cryptography 
**  Author: Alfred J. Menezes, Paul C. van Oorschot and Scott A. Vanstone
**  Page: 608, Alg. 14.61 binary extended-gcd algorithm
**  input: x ,y
**  output: x^-1 mod y
**  return: inv
**/
void inverse(uint16_t *inv, uint16_t* x, uint16_t* y){
    uint16_t* g = malloc(WORD_LENGTH*sizeof(uint16_t));
    D{printf("Start compute inverse\n");}
    for(uint8_t i=0; i<WORD_LENGTH; ++i){
        g[i]=0;
    }
    g[1]=0;
    while((x[0]&1u)==0 && (y[0]&1u)==0){
        half(x); half(y);times_two(g);
        D{printf("After step 2\n"); printf("x: \n");print_bin(x,WORD_LENGTH);printf("y: \n");print_bin(y,WORD_LENGTH);printf("g: \n");print_bin(g,WORD_LENGTH);};
    }
    uint16_t* u = malloc(WORD_LENGTH*sizeof(uint16_t));
    uint16_t* v = malloc(WORD_LENGTH*sizeof(uint16_t));
    uint16_t* a = malloc(WORD_LENGTH*sizeof(uint16_t)); uint8_t a_neg = 0;
    uint16_t* b = malloc(WORD_LENGTH*sizeof(uint16_t)); uint8_t b_neg = 0;
    uint16_t* c = malloc(WORD_LENGTH*sizeof(uint16_t)); uint8_t c_neg = 0;
    uint16_t* d = malloc(WORD_LENGTH*sizeof(uint16_t)); uint8_t d_neg = 0;
    uint8_t zero = 0;
    for(uint8_t i =0; i<WORD_LENGTH; ++i){
        u[i]=x[i]; v[i]=y[i]; a[i]=0; c[i]=0; b[i]=0; d[i]=0;
    }
    d[0]=1;a[0]=1;
    uint8_t carry =0;
    while(isZero(u)==0){
        D{char str[1]; gets(str);}
        while((u[0]&1U) == 0){
            half(u);
            if((a[0]&1u)==0 && (b[0]&1u)==0){
                half(a);half(b);
            }else{
                sign_add(a,y,&a_neg,&zero);half(a);
                sign_sub(b,x,&b_neg,&zero);half(b);
            }
            D{printf("Step 4:\n"); printf("u: 2\n");print_dec(u,WORD_LENGTH);}
            D{printf("a: \n");print_dec(a,WORD_LENGTH);printf("b: \n");print_dec(b,WORD_LENGTH);puts("");}
        }
        while((v[0]&1U) == 0){
            half(v);
            if((c[0]&1u)==0 && (d[0]&1u)==0){
                half(c);half(d);
            }else{
                sign_add(c,y,&c_neg,&zero);half(c);
                sign_sub(d,x,&d_neg,&zero);half(d);
            }
             D{printf("Step 5 end:\n"); printf("v: 2\n");print_dec(v,WORD_LENGTH);}
             D{printf("c: 2\n");print_dec(c,WORD_LENGTH);printf("d: 2\n");print_dec(d,WORD_LENGTH);puts("");}
        }
        if(compare(u,v)>=0){
            D{printf("u>=v\n");printf("v: 2\n");print_dec(v,WORD_LENGTH);printf("u: 2\n");print_dec(u,WORD_LENGTH);}
            sub(u,v,&carry);sign_sub(a,c,&a_neg,&c_neg);sign_sub(b,d,&b_neg,&d_neg);
        }else{
            D{printf("u<v\n");printf("v: 2\n");print_dec(v,WORD_LENGTH);printf("u: 2\n");print_dec(u,WORD_LENGTH);}
            sub(v,u,&carry);sign_sub(c,a,&c_neg,&a_neg);sign_sub(d,b,&d_neg,&b_neg);
        }
        D{printf("Step 6:\n"); printf("b: 2\n");print_dec(b,WORD_LENGTH);printf("d: 2\n");print_dec(d,WORD_LENGTH);}
        D{printf("a: 2\n");print_dec(a,WORD_LENGTH);printf("c: 2\n");print_dec(c,WORD_LENGTH);}
        D{printf("u: 2\n");print_dec(u,WORD_LENGTH);printf("v: 2\n");print_dec(v,WORD_LENGTH);puts("");}
    }
    for(uint8_t i =0; i< WORD_LENGTH; ++i){
        inv[i] = d[i];
    }
    free(g);free(u);free(v);free(a);free(b);free(c);free(d);
}

/**
**  Ref: Handbook of Applied Cryptography 
**  Author: Alfred J. Menezes, Paul C. van Oorschot and Scott A. Vanstone
**  Page: 598, Alg. 14.20 Multiple-precision division
**  input: x ,y, n = word length of x , t = word length of y
**  output: x/y
**  return: q = quotient, r =reminder
**/
void divide(uint16_t* q, uint16_t* r, uint16_t*  x, uint16_t*  y, uint8_t n, uint8_t t){
    for(uint8_t i = 0; i<=(n-t); ++i){
        q[i] = 0;
    }
    uint16_t* xc = malloc((n+1) * sizeof(uint16_t));
    for(uint8_t i = 0; i<=n; ++i){
        xc[i] = x[i];
    }
    uint16_t* yc = malloc((n+1) * sizeof(uint16_t));
    for(uint8_t i=0; i<=n; ++i){
        if(i < n-t){
            yc[i] = 0;
        }else{
            yc[i] = y[i-(n-t)];
        }
    }
    D{printf("xc: ");print_hex(xc, n+1); printf("yc: ");print_hex(yc, n+1);}
    uint8_t carry = 0;
    while(compare_len(xc, yc, n+1)>=0){
        q[n-t] += 1;
        sub_len(xc, yc, &carry, n+1);
    }
    D{printf("after step 2: \n"); printf("xc: ");print_hex(xc, n+1); printf("q: ");print_hex(q, (n-t+1));}
    for(uint8_t i=n; i>=t+1; --i){
        if(xc[i] == y[t]){
            q[i-t-1] = 255;
        }else{
            uint16_t tmp = xc[i]<<8; // x[i] * 256
            tmp += xc[i-1];
            tmp = tmp / y[t];
            q[i-t-1] = tmp & 0xff;
        }
        D{printf("after step 3.1: \n"); printf("q: ");print_hex(q, (n-t+1));}
        if(t>0 && i>=2){
            while(1){
                uint32_t tmp = y[t]<<8;
                tmp += y[t-1];
                tmp = tmp * q[i-t-1];
                D{printf("tmp : %d\n", tmp);}
                uint32_t tmp2 = (xc[i]<<16) + (xc[i-1]<< 8)+ xc[i-2];
                D{printf("tmp2: %d\n", tmp2);}
                if(tmp>tmp2){
                    q[i-t-1]--;
                }else{
                    break;
                }
                D{printf("after step 3.2: \n"); printf("q: ");print_hex(q, (n-t+1));}
            }
        }
        uint16_t* tmp = malloc(WORD_LENGTH*2*sizeof(uint16_t));
        uint16_t* qarr = malloc(WORD_LENGTH*sizeof(uint16_t));
        qarr[0] = q[i-t-1];
        mult(tmp, y, qarr);
        for(uint8_t j=0; j<=n; j++){
            yc[j] = 0;
        }
        for(uint8_t j=i-t-1; j<=n; j++){
            if((j-(i-t-1))<WORD_LENGTH*2){
                yc[j] = tmp[j-(i-t-1)];
            }
        }
        carry = 0;
        D{printf("before step 3.3: \n"); printf("xc: ");print_hex(xc, (n+1)); printf("yc: ");print_hex(yc, n+1);}
        sub_len(xc, yc, &carry, n+1);
        D{printf("after step 3.3: \n");printf("xc: ");print_hex(xc, (n+1)); printf("yc: ");print_hex(yc, n+1); printf("q: ");print_hex(q, (n-t+1));}
        if(carry>0){
            for(uint8_t j=0; j<=n; j++){
                yc[j] = 0;
            }
            for(uint8_t j=i-t-1; j<=n; j++){
                if((j-(i-t-1))<t){
                    yc[j] = y[j-(i-t-1)];
                }
            }
            carry = 0;
            D{printf("before step 3.4: \n"); printf("xc: ");print_hex(xc, (n+1)); printf("yc: ");print_hex(yc, n+1);}
            add_len(xc, yc, &carry, n+1);
            D{printf("after step 3.4: \n"); printf("xc: ");print_hex(xc, (n+1)); printf("yc: ");print_hex(yc, n+1);}
            q[i-t-1]--;
            D{printf("q: ");print_hex(q, (n-t+1));}
        }
        free(tmp);free(qarr);
    }
    for(int i=0; i<t+1; ++i){
        r[i] = xc[i];
    }
    D{printf("r: ");print_hex(r, (t+1));}
    free(xc);free(yc);
}