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
    case natural_e:
        return strnatcmp;
    case ci_e:
        return strcasecmp;
    case natural_e | ci_e:
        return strnatcasecmp;
    case desc_e:
        return cmp_str_desc;
    case natural_e | desc_e:
        return cmp_str_nat_desc;
    case ci_e | desc_e:
        return cmp_str_case_desc;
    case natural_e | ci_e | desc_e:
        return cmp_str_nat_case_desc;
    case 0:
    default:
        return strcmp;
    }
}
