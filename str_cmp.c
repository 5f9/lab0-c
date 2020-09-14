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

cmp_func get_compar(const size_t order_bits)
{
    switch (order_bits) {
    case NATURAL_E:
        return strnatcmp;
    case CI_E:
        return strcasecmp;
    case NATURAL_E | CI_E:
        return strnatcasecmp;
    case DESC_E:
        return cmp_str_desc;
    case NATURAL_E | DESC_E:
        return cmp_str_nat_desc;
    case CI_E | DESC_E:
        return cmp_str_case_desc;
    case NATURAL_E | CI_E | DESC_E:
        return cmp_str_nat_case_desc;
    case 0:
    default:
        return strcmp;
    }
}
