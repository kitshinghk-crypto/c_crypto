#include "arithmetic.h"
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#define D if(ARITH_DEBUG)

bool ARITH_DEBUG = false;

/**  Assume word length = 8
**   01 00 00 00 00 00 00 00
**   0  1  2  3  4  5  6  7
**   LSB                  LMB
**/  
//uint8_t WORD_LENGTH = 32;

void print_dec(const uint16_t* b, uint8_t len){
    for(int i = len-1; i>=0; --i){
        printf("%d ", b[i]&0x00ff);
    }
    puts("");
}

void print_hex(const uint16_t* b, uint8_t len){
    for(int i = len-1; i>=0; --i){
        printf("%02x", b[i]&0x00ff);
    }
    puts("");
}

void print_bin(const uint16_t* ptr, uint8_t size){
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

int compare_len(const uint16_t *a, const uint16_t*b, uint8_t len){
    for(uint8_t i = len-1; i>=0; i--){
        if(a[i]>b[i]){
            return 1;
        }else if(a[i]<b[i]){
            return -1; 
        }
    }
    return 0;
}

void copy(uint16_t* a, const uint16_t* b){
    copy_len(a,b,WORD_LENGTH);
}

void copy_len(uint16_t* a, const uint16_t* b, size_t len){
    for(int i = len-1; i>=0; --i){
        a[i] = b[i];
    }
}

// a>b, return 1
// a<b, return -1
// a=b, return 0
int compare(const uint16_t *a, const uint16_t*b){
    for(int i = WORD_LENGTH-1; i>=0; i--){
        if(a[i]>b[i]){
            return 1;
        }else if(a[i]<b[i]){
            return -1; 
        }
    }
    return 0;
}

int is_zero(const uint16_t *a){
    for(uint8_t i = 0; i<WORD_LENGTH; i++){
        if((a[i]&0xff) > 0){
            return 0;
        }
    }
    return 1;
}

int is_one(const uint16_t *a){
    for(uint8_t i = 1; i<WORD_LENGTH; i++){
        if((a[i]&0xff) > 0){
            return 0;
        }
    }
    return (a[0]&0xff)==1 ;
}

int is_equal(const uint16_t *x,const uint16_t *y){
    for(uint8_t i = 1; i<WORD_LENGTH; i++){
        if(x[i]!=y[i]){
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
void add(uint16_t* a, const uint16_t* b, uint8_t* carry){
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

void add_len(uint16_t* a, const uint16_t* b, uint8_t* carry, uint8_t len){
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
void mult(uint16_t*c, const uint16_t* a, const uint16_t* b){
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
void sub(uint16_t* a, const uint16_t* b, uint8_t* carry){
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

void sub_len(uint16_t* a, const uint16_t* b, uint8_t* carry, uint8_t len){
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
void mod_add(uint16_t *a, const uint16_t *b, const uint16_t* p){
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
void mod_sub(uint16_t *a, const uint16_t *b, const uint16_t* p){
    uint8_t carry=0;
    sub(a,b,&carry);
    if (carry == 1){
        carry = 0;
        add(a, p, &carry);
    }
}

void neg(uint16_t *x){
    uint16_t z[WORD_LENGTH] = {0};
    for(uint8_t i =0; i<WORD_LENGTH; i++){
        z[i] = 0;
    }
    uint8_t carry =0;
    sub(z, x, &carry);
    for(uint8_t i =0; i<WORD_LENGTH; i++){
        x[i] = z[i];
    }
}

void sign_sub(uint16_t* x , const uint16_t* y, uint8_t* x_neg, uint8_t* y_neg){
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

void sign_add(uint16_t* x , const uint16_t* y, uint8_t* x_neg, uint8_t* y_neg){
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
**  output: y^-1 mod x
**  return: inv
**/
void inverse(uint16_t *inv, const uint16_t* x, const uint16_t* y){
    uint16_t g[WORD_LENGTH] = {0};
    uint16_t xc[WORD_LENGTH] = {0};
    uint16_t yc[WORD_LENGTH] = {0};
    for(size_t i=0; i< WORD_LENGTH; i++){
        xc[i]=x[i];yc[i]=y[i];
    }
    D{printf("Start compute inverse\n");}
    for(uint8_t i=0; i<WORD_LENGTH; ++i){
        g[i]=0;
    }
    g[0]=1;
    D{printf("Before step 2\n"); printf("x: \n");print_bin(x,WORD_LENGTH);printf("y: \n");print_bin(y,WORD_LENGTH);}
    while((xc[0]&1u)==0 && (yc[0]&1u)==0){
        half(xc); half(yc);times_two(g);
        D{printf("After step 2\n"); printf("x: \n");print_bin(x,WORD_LENGTH);printf("y: \n");print_bin(y,WORD_LENGTH);printf("g: \n");print_bin(g,WORD_LENGTH);};
    }
    uint16_t u[WORD_LENGTH]= {0};
    uint16_t v[WORD_LENGTH]= {0};
    uint16_t a[WORD_LENGTH]= {0}; uint8_t a_neg = 0;
    uint16_t b[WORD_LENGTH]= {0}; uint8_t b_neg = 0;
    uint16_t c[WORD_LENGTH]= {0}; uint8_t c_neg = 0;
    uint16_t d[WORD_LENGTH]= {0}; uint8_t d_neg = 0;
    uint8_t zero = 0;
    for(uint8_t i =0; i<WORD_LENGTH; ++i){
        u[i]=x[i]; v[i]=y[i]; 
    }
    d[0]=1;a[0]=1;
    uint8_t carry =0;
    while(is_zero(u)==0){
        D{char str[1]; gets(str);}
        while((u[0]&1U) == 0){
            half(u);
            if((a[0]&1u)==0 && (b[0]&1u)==0){
                half(a);half(b);
            }else{
                sign_add(a,y,&a_neg,&zero);half(a);
                sign_sub(b,x,&b_neg,&zero);half(b);
            }
            D{printf("Step 4:\n"); printf("u: \n");print_dec(u,WORD_LENGTH);}
            D{printf("a: %u\n",a_neg);print_dec(a,WORD_LENGTH);printf("b: %u\n",b_neg);print_dec(b,WORD_LENGTH);puts("");}
        }
        while((v[0]&1U) == 0){
            half(v);
            if((c[0]&1u)==0 && (d[0]&1u)==0){
                half(c);half(d);
            }else{
                sign_add(c,y,&c_neg,&zero);half(c);
                sign_sub(d,x,&d_neg,&zero);half(d);
            }
             D{printf("Step 5 end:\n"); printf("v: \n");print_dec(v,WORD_LENGTH);}
             D{printf("c: %u\n",c_neg);print_dec(c,WORD_LENGTH);printf("d: %u\n",d_neg);print_dec(d,WORD_LENGTH);puts("");}
        }
        if(compare(u,v)>=0){
            D{printf("u>=v\n");printf("v: \n");print_dec(v,WORD_LENGTH);printf("u: \n");print_dec(u,WORD_LENGTH);}
            sub(u,v,&carry);sign_sub(a,c,&a_neg,&c_neg);sign_sub(b,d,&b_neg,&d_neg);
        }else{
            D{printf("u<v\n");printf("v: \n");print_dec(v,WORD_LENGTH);printf("u: \n");print_dec(u,WORD_LENGTH);}
            sub(v,u,&carry);sign_sub(c,a,&c_neg,&a_neg);sign_sub(d,b,&d_neg,&b_neg);
        }
        D{printf("Step 6:\n"); printf("b: %u\n",b_neg);print_dec(b,WORD_LENGTH);printf("d: %u\n",d_neg);print_dec(d,WORD_LENGTH);}
        D{printf("a: %u\n",a_neg);print_dec(a,WORD_LENGTH);printf("c: %u\n",c_neg);print_dec(c,WORD_LENGTH);}
        D{printf("u: \n");print_dec(u,WORD_LENGTH);printf("v: \n");print_dec(v,WORD_LENGTH);puts("");}
    }
    for(uint8_t i =0; i< WORD_LENGTH; ++i){
        inv[i] = d[i];
    }

    if(d_neg == 1){
        copy(inv, x);
        sub(inv, d, &carry);
    }else{
        copy(inv, d);
    }
}

/**
**  Ref: Handbook of Applied Cryptography 
**  Author: Alfred J. Menezes, Paul C. van Oorschot and Scott A. Vanstone
**  Page: 598, Alg. 14.20 Multiple-precision division
**  input: x ,y, n = word length of x , t = word length of y
**  output: x/y
**  return: q = quotient, r =reminder
**/
void divide(uint16_t* q, uint16_t* r, const uint16_t*  x, const uint16_t*  y, uint8_t n, uint8_t t){
    uint16_t* xc = malloc(sizeof(uint16_t) * (n+1));
    uint16_t* yc = malloc(sizeof(uint16_t) * (n+1));
    uint16_t qarr[WORD_LENGTH] = {0};
    uint16_t tmp[WORD_LENGTH*2] = {0};
    
    for(uint8_t i = 0; i<=n; ++i){
        xc[i] = x[i];
    }
    for(uint8_t i=0; i<=n; ++i){
        if(i < n-t){
            yc[i] = 0;
        }else{
            yc[i] = y[i-(n-t)];
        }
    }
    D{printf("xc: ");print_hex(xc, n+1); printf("yc: ");print_hex(yc, n+1); printf("n: %u\n",n);printf("t: %u\n",t);}
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
        for(int i=0;i<WORD_LENGTH*2; i++){
            tmp[i]=0;
        }
        for(int i=0;i<WORD_LENGTH; i++){
            qarr[i]=0;
        }
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
                if((j-(i-t-1))<=t){
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
    }
    for(int i=0; i<t+1; ++i){
        r[i] = xc[i];
    }
    D{printf("r: ");print_hex(r, (t+1));}
    free(xc);free(yc);xc=NULL;yc=NULL;
}

void reduce(uint16_t* x, uint8_t xlen, const uint16_t* mod){
    int n;
    int t;
    for(n=xlen-1; n>=0; n--){
        if(x[n]!=0){
            break;
        }
    }
    for(t=WORD_LENGTH-1; t>=0; t--){
        if(mod[t]!=0){
            break;
        }
    }
    if(n<t){
        return;
    }
    uint16_t q[WORD_LENGTH] = {0};
    uint16_t r[WORD_LENGTH] = {0};
    divide(q,r,x,mod,n,t);
    for(int i =0; i<WORD_LENGTH; i++){
        x[i]=r[i];
    }
}

void mod_mult(uint16_t* a, const uint16_t* b, const uint16_t* n){
    uint16_t c[WORD_LENGTH*2] = {0};
    mult(c,a,b);
    reduce(c, WORD_LENGTH * 2, n);
    for(int i=0; i<WORD_LENGTH; i++){
        a[i]=c[i];
    }
}

/**
**  Ref: Guide to elliptic curve cryptography 
**  Author: Hankerson, D., Menezes, A.J. and Vanstone, S.
**  Page: 40, Algorithm 2.20 Inversion in Fp using the extended Euclidean algorithm
**  input: a,p
**  output: a^-1 mod p
**  return: inv
**/
void inv_p(uint16_t* inv, const uint16_t* a, const uint16_t* p){
    uint16_t u[WORD_LENGTH] = {0};
    uint16_t v[WORD_LENGTH] = {0};
    uint16_t x[WORD_LENGTH] = {0};
    uint16_t x1[WORD_LENGTH] = {0};
    uint16_t x2[WORD_LENGTH] = {0};
    uint16_t q[WORD_LENGTH] = {0};
    uint16_t r[WORD_LENGTH] = {0};
    uint16_t prod[WORD_LENGTH*2] = {0};

    copy(u,a); copy(v,p); x1[0]=1; x2[0]=0;
    int n,t; uint8_t carry=0;uint8_t zero = 0;
    uint8_t x_neg=0;uint8_t x1_neg =0;uint8_t x2_neg=0;
    D{printf("Compute inverse:\n");}
    D{printf("a:\n"); print_hex(a, WORD_LENGTH);}
    D{printf("p:\n"); print_hex(p, WORD_LENGTH);}
    while (is_one(u)!=1){
        D{printf("u:\n"); print_hex(u, WORD_LENGTH);}
         D{char str[1]; gets(str);}
        for(n=WORD_LENGTH-1; n>=0; n--){
            if(v[n]!=0){
                break;
            }
        }
        for(t=WORD_LENGTH-1; t>=0; t--){
            if(u[t]!=0){
                break;
            }
        }
        divide(q,r,v,u,n,t);
        D{printf("q:\n"); print_hex(q, WORD_LENGTH);}
        D{printf("r:\n"); print_hex(r, WORD_LENGTH);}
        copy(x,x2);
        for(int i =0; i<2*WORD_LENGTH; i++){
            prod[i] =0;
        }
        //ARITH_DEBUG = false;
        mult(prod,q,x1);
        //ARITH_DEBUG = true;
        D{printf("prod:\n"); print_hex(prod, WORD_LENGTH);}
        copy(x,x2);x_neg = x2_neg;
        sign_sub(x,prod,&x_neg,&x1_neg);
        D{printf("x: %d\n",x_neg); print_hex(x, WORD_LENGTH);}
        copy(v,u);copy(u,r);
        copy(x2,x1);x2_neg=x1_neg;
        copy(x1,x);x1_neg=x_neg;
        for(int i=0; i< WORD_LENGTH; i++){
            q[i]=0;
        }
    }
    if(x_neg == 1){
        copy(inv, p);
        sub(inv, x, &carry);
    }else{
        copy(inv, x);
    }
    
}

void mont_mult(uint16_t* x, uint16_t* y,uint16_t* m,uint16_t m_pi, size_t n){
    D{printf("x:"); print_hex(x,WORD_LENGTH); printf("y:"); print_hex(y,WORD_LENGTH); printf("m:"); print_hex(m,WORD_LENGTH);printf("m_pi:%02x\n",m_pi);}
    uint16_t* a = malloc(sizeof(uint16_t)*(n+1));
    for(size_t i=0; i<n+1; i++){
        a[i] = 0;
    }
    uint16_t u = 0;
    uint16_t tmp1[WORD_LENGTH*2] = {0};
    uint16_t tmp2[WORD_LENGTH] = {0};
    uint8_t carry = 0;
    for(int i =0;i<n;i++){
        u = (x[i] * y[0]) &0xff;
        u = (u + a[0]) & 0xff;
        u = (u * m_pi) & 0xff;
        D{printf("i=%d, u=%02x\n",i,u);}
        for(size_t i=0; i<WORD_LENGTH*2; i++){
            tmp1[i] =0; 
            if(i<WORD_LENGTH){
                tmp2[i]=0;
            }
        }
        tmp2[0] = x[i];
        mult(tmp1,tmp2,y);
        D{printf("x[%d] * y:\n",i); print_hex(tmp1, WORD_LENGTH*2);}
        add_len(a, tmp1, &carry, n+1);
        D{printf("carry=%u\n",carry);}
        for(size_t i=0; i<WORD_LENGTH*2; i++){
            tmp1[i] =0; 
            if(i<WORD_LENGTH){
                tmp2[i]=0;
            }
        }
        tmp2[0] = u;
        mult(tmp1,tmp2,m);
        D{printf("u[%d] * m:\n",i); print_hex(tmp1, WORD_LENGTH*2);}
        add_len(a, tmp1, &carry, n+1);
        D{printf("carry=%u\n",carry) ;}
        D{printf("a:\n"); print_hex(a, n+1);}
        for(int i=0;i<n+1;i++){
            a[i]=a[i+1];
        }
        a[n]=carry;
        carry=0;
        D{printf("a/2:\n"); print_hex(a, n+1);}
    }
    if(a[n]==1 || compare(a,m)>=0){
        sub(a, m, &carry);
        copy_len(x,a,WORD_LENGTH);
    }else{
        copy_len(x,a,WORD_LENGTH);
    }
}


void mont_exp(uint8_t* x,const uint8_t* e,const uint8_t* p,size_t rlen){
    uint16_t x_mont[WORD_LENGTH] = {0};
    uint16_t* r = malloc(sizeof(uint16_t) * (rlen+1)); r[rlen]=1;
    uint16_t a[WORD_LENGTH] = {0};
    uint16_t a_copy[WORD_LENGTH] = {0};
    uint16_t one[WORD_LENGTH] = {0}; one[0]=1;
    uint16_t p16[WORD_LENGTH] = {0};
    uint16_t * r16 = malloc(sizeof(uint16_t)*(rlen+1));
    uint16_t m_pi =0;
    for(m_pi=0; m_pi<256; m_pi++){
        if((m_pi*(p[0]&0xff))%256 == 1){
            break;
        }
    }
    m_pi = 256 - m_pi;
    for(size_t i=0;i<WORD_LENGTH;i++){
        x_mont[i] = x[i]; p16[i] = p[i];
    }
    for(size_t i=0; i<rlen+1; i++){
        r16[i] = r[i];
    }
    reduce(r16, rlen+1, p16);
    mod_mult(x_mont, r16, p16);
    copy(a, r16);
    int start_ind = 255;
    for(start_ind=255; start_ind>=0 ;start_ind--){
        uint8_t bit = (e[start_ind/8] >> (start_ind%8)) & 1U;
        if(bit){
            break;
        }
    }
    for(int i=start_ind;i>=0;i--){
        uint8_t bit = (e[i/8] >> (i%8)) & 1U;
        copy(a_copy, a);
            ARITH_DEBUG =true;
        mont_mult(a,a_copy,p16,m_pi,WORD_LENGTH);
            ARITH_DEBUG =false;
        if(bit == 1){
            mont_mult(a,x_mont,p16,m_pi,WORD_LENGTH);
        }
    }
    mont_mult(a,one,p16,m_pi,WORD_LENGTH);
    for(int i=0; i<WORD_LENGTH; i++){
        x[i] = a[i] & 0xff;
    }
} 