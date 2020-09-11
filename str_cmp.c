#include "str_cmp.h"

// cppcheck-suppress unusedFunction
int cmp_str_desc(const char *a, const char *b)
{
    return strcmp(b, a);
}

// cppcheck-suppress unusedFunction
int cmp_str_nat_desc(const char *a, const char *b)
{
    return strnatcmp(b, a);
}

// cppcheck-suppress unusedFunction
int cmp_str_case_desc(const char *a, const char *b)
{
    return strcasecmp(b, a);
}

// cppcheck-suppress unusedFunction
int cmp_str_nat_case_desc(const char *a, const char *b)
{
    return strnatcasecmp(b, a);
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
                return strnatcasecmp;
            } else {
                return strcasecmp;
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
                return strnatcmp;
            } else {
                return strcmp;
            }
        }
    }
}