#ifndef ECURVE_H
#define ECURVE_H
#include <stdint.h>
typedef int bool;
#define true 1
#define false 0
extern bool ECURVE_DEBUG;

struct epoint{
    uint16_t* x;
    uint16_t* y;
};

struct epoint_proj{
    uint16_t* x;
    uint16_t* y;
    uint16_t* z;
};

extern struct epoint* epoint_init();
extern struct epoint_proj* epoint_proj_init();
extern void print_epoint(const struct epoint*  );
extern void print_epoint_proj(const struct epoint_proj* );
extern int p256_point_double(struct epoint_proj* );
extern int p256_point_add(struct epoint_proj* , struct epoint* );
#endif