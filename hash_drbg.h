#ifndef HASH_DRBG_H
#define HASH_DRBG_H
#include <stdint.h>
#include <stddef.h>
typedef int bool;
#define true 1
#define false 0
extern bool HASH_DRBG_DEBUG;

struct hashdrbg{
    uint8_t* v;
    size_t vlen;
    uint8_t* c;
    size_t clen;
    size_t reseed_counter;
};

extern struct hashdrbg* hashdrbg_init(const uint8_t* entropy_input, size_t elen, const uint8_t* nonce, size_t nlen);
extern void hashdrbg_reseed(struct hashdrbg* h, const uint8_t* entropy_input, size_t elen, const uint8_t* additional_input, size_t alen);
extern uint8_t* hashdrbg_next(struct hashdrbg* h, size_t requested_no_of_bit, const uint8_t* additional_input, size_t alen);
#endif