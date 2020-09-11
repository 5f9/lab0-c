#ifndef STR_CMP_H
#define STR_CMP_H

#include <string.h>
#include <strings.h>
#include "natsort/strnatcmp.h"

/* Comparison of the Linked list element function */
typedef int (*cmp_func)(const char *, const char *);

cmp_func q_get_compar(const int sorting_order,
                      const int natural_sort,
                      const int ignoring_case);

#endif