#include "str_cmp.h"
#include "natsort/strnatcmp.h"

// cppcheck-suppress unusedFunction
int cmp_str(const void *a, const void *b)
{
    return strcmp(*(char **) a, *(char **) b);
}

// cppcheck-suppress unusedFunction
int cmp_str_desc(const void *a, const void *b)
{
    return strcmp(*(char **) b, *(char **) a);
}

// cppcheck-suppress unusedFunction
int cmp_str_nat(const void *a, const void *b)
{
    return strnatcmp(*(char **) a, *(char **) b);
}

// cppcheck-suppress unusedFunction
int cmp_str_nat_desc(const void *a, const void *b)
{
    return strnatcmp(*(char **) b, *(char **) a);
}

// cppcheck-suppress unusedFunction
int cmp_str_case(const void *a, const void *b)
{
    return strcasecmp(*(char **) a, *(char **) b);
}

// cppcheck-suppress unusedFunction
int cmp_str_case_desc(const void *a, const void *b)
{
    return strcasecmp(*(char **) b, *(char **) a);
}

// cppcheck-suppress unusedFunction
int cmp_str_nat_case(const void *a, const void *b)
{
    return strnatcasecmp(*(char **) a, *(char **) b);
}

// cppcheck-suppress unusedFunction
int cmp_str_nat_case_desc(const void *a, const void *b)
{
    return strnatcasecmp(*(char **) b, *(char **) a);
}

cmp_func q_get_compar(const int sorting_order,
                      const int natural_sort,
                      const int ignoring_case)
{
    if (ignoring_case) {
        if (sorting_order) {
            if (natural_sort) {
                return cmp_str_nat_case_desc;
            } else {
                return cmp_str_case_desc;
            }
        } else {
            if (natural_sort) {
                return cmp_str_nat_case;
            } else {
                return cmp_str_case;
            }
        }
    } else {
        if (sorting_order) {
            if (natural_sort) {
                return cmp_str_nat_desc;
            } else {
                return cmp_str_desc;
            }
        } else {
            if (natural_sort) {
                return cmp_str_nat;
            } else {
                return cmp_str;
            }
        }
    }
}