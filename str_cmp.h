#ifndef STR_CMP_H
#define STR_CMP_H

#include <strings.h>
#include "natsort/strnatcmp.h"

/* Comparison of the Linked list element function */
typedef int (*cmp_func)(const void *, const void *);

int cmp_str(const void *a, const void *b);
int cmp_str_desc(const void *a, const void *b);
int cmp_str_nat(const void *a, const void *b);
int cmp_str_nat_desc(const void *a, const void *b);
cmp_func q_get_compar(const int sorting_order, const int natural_sort);

#endif
