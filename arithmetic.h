#ifndef ARITHMETIC_H
#define ARITHMETIC_H
#include <stdint.h>
#include <stddef.h>
typedef int bool;
#define true 1
#define false 0
extern uint8_t  WORD_LENGTH;
extern bool ARITH_DEBUG;
extern void add(uint16_t *, uint16_t *, uint8_t* );
extern void sub(uint16_t *, uint16_t *, uint8_t* );
extern void sign_sub(uint16_t* , uint16_t*, uint8_t*, uint8_t*);
extern void sign_add(uint16_t* , uint16_t* , uint8_t* , uint8_t*);
extern void set_zero(uint16_t*);
extern void mult(uint16_t*, uint16_t* , uint16_t* );
extern void divide(uint16_t*, uint16_t*, uint16_t*, uint16_t*, uint8_t, uint8_t);
extern void half(uint16_t*);
extern void neg(uint16_t*);
extern void times_two(uint16_t*);
extern void mod_add(uint16_t *, uint16_t *, uint16_t* );
extern void mod_sub(uint16_t *, uint16_t *, uint16_t* );
extern void inverse(uint16_t *, uint16_t*, uint16_t* );
extern void reduce(uint16_t*, uint16_t*);
extern int compare(uint16_t *, uint16_t*);
extern void print_hex(uint16_t *, uint8_t);
extern void print_bin(uint16_t *, uint8_t);
extern void print_dec(uint16_t *, uint8_t);
#endif