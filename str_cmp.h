#ifndef LAB0_STR_CMP_H
#define LAB0_STR_CMP_H

#include <string.h>
#include <strings.h>
#include "natsort/strnatcmp.h"

typedef enum {
    natural_e = 0x01,  // natural sort
    ci_e = 0x02,       // case-insensitive
    desc_e = 0x04,     // descending order
} order_e;

/* Comparison of the Linked list element function */
typedef int (*cmp_func)(const char *, const char *);

cmp_func get_compar(const size_t order_bits);

#endif