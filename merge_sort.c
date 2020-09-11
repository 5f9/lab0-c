#include "queue.h"

static void _merge_sort(list_ele_t **x, cmp_func cmp)
{
    list_ele_t *head = *x;
    if (!(head && head->next))
        return;

    list_ele_t *a = head->next;
    list_ele_t *b = head;

    while (a && a->next) {
        b = b->next;
        a = a->next->next;
    }
    a = b->next;
    b->next = NULL;
    b = head;

    _merge_sort(&b, cmp);
    _merge_sort(&a, cmp);

    *x = NULL;
    list_ele_t **tmp = x;

    while (a && b) {
        if (cmp(&a->value, &b->value) <= 0) {
            *tmp = a;
            a = a->next;
        } else {
            *tmp = b;
            b = b->next;
        }
        tmp = &((*tmp)->next);
    }

    *tmp = a ? a : b;
}

void merge_sort(queue_t *q, cmp_func cmp)
{
    _merge_sort(&q->head, cmp);
    list_ele_t *e = q->tail;
    while (e->next) {
        e = e->next;
    }
    q->tail = e;
}
