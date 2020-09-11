#include <stdlib.h>
#include "queue.h"

static char *q_sort_array[2097152];
// cppcheck-suppress unusedFunction
void quick_sort(queue_t *q, cmp_func cmp_fun)
{
    // char *q_sort_array[q->size];
    list_ele_t *p = q->head;
    for (size_t i = 0; i < q->size; i++) {
        q_sort_array[i] = p->value;
        p = p->next;
    }
    qsort(q_sort_array, q->size, sizeof(char *), cmp_fun);

    p = q->head;
    for (size_t i = 0; i < q->size; i++) {
        p->value = q_sort_array[i];
        p = p->next;
    }
}
