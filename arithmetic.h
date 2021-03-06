#ifndef ARITHMETIC_H
#define ARITHMETIC_H
#include <stdint.h>
#include <stddef.h>
typedef int bool;
#define true 1
#define false 0
#define WORD_LENGTH 256

//extern uint8_t  WORD_LENGTH;
extern bool ARITH_DEBUG;
extern void add(uint16_t *, const uint16_t *, uint8_t* );
extern void sub(uint16_t *, const uint16_t *, uint8_t* );
extern void sign_sub(uint16_t* , const uint16_t*, uint8_t*, uint8_t*);
extern void sign_add(uint16_t* , const uint16_t* , uint8_t* , uint8_t*);
extern void set_zero(uint16_t*);
extern void mult(uint16_t*, const uint16_t* , const uint16_t* );
extern void divide(uint16_t*, uint16_t*, const uint16_t*, const uint16_t*, size_t, size_t);
extern void half(uint16_t*);
extern void neg(uint16_t*);
extern void times_two(uint16_t*);
extern void mod_add(uint16_t *, const uint16_t *, const uint16_t* );
extern void mod_sub(uint16_t *, const uint16_t *, const uint16_t* );
extern void mod_mult(uint16_t*, const uint16_t*, const uint16_t*);
extern void inverse(uint16_t *, const uint16_t*, const uint16_t* );
extern void inv_p(uint16_t* , const uint16_t* , const uint16_t* );
extern void reduce(uint16_t*, size_t, const uint16_t*);
extern int compare(const uint16_t *, const uint16_t*);
extern void copy(uint16_t*, const uint16_t*);
extern void copy_len(uint16_t*, const uint16_t*, size_t);
extern void print_hex(const uint16_t *, size_t);
extern void print_bin(const uint16_t *, size_t);
extern void print_dec(const uint16_t *, size_t);
extern int is_zero(const uint16_t *);
extern int is_one(const uint16_t *);
extern int is_equal(const uint16_t *,const uint16_t *);
extern void mont_mult(uint16_t* x, const uint16_t* y, const uint16_t* m,uint16_t m_pi, size_t n);
extern void mont_exp(uint16_t* x,const uint16_t* e,const uint16_t* p,size_t klen,size_t rlen);
#endif