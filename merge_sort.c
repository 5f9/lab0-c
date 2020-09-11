// SPDX-License-Identifier: GPL-2.0
#include "queue.h"

// From https://github.com/torvalds/linux/blob/master/lib/list_sort.c
/*
 * Returns a list organized in an intermediate format suited
 * to chaining of merge() calls: null-terminated, no reserved or
 * sentinel head node.
 */
static inline list_ele_t *merge(cmp_func cmp, list_ele_t *a, list_ele_t *b)
{
    // cppcheck-suppress unassignedVariable
    list_ele_t *head, **tail = &head;

    for (;;) {
        /* if equal, take 'a' -- important for sort stability */
        if (cmp(&a->value, &b->value) <= 0) {
            *tail = a;
            tail = &(a->next);
            a = a->next;
            if (!a) {
                *tail = b;
                break;
            }
        } else {
            *tail = b;
            tail = &b->next;
            b = b->next;
            if (!b) {
                *tail = a;
                break;
            }
        }
    }
    return head;
}

static void _merge_sort(list_ele_t **x, size_t size, cmp_func cmp)
{
    list_ele_t *head = *x;
    RETURN_IF_NULL(head && head->next, );

    size_t count = size >> 1;
    list_ele_t *b = head;
    for (size_t i = 1; i < count; i++)
        b = b->next;

    list_ele_t *a = b->next;
    b->next = NULL;
    b = head;

    _merge_sort(&b, count, cmp);
    _merge_sort(&a, count + (size & 1), cmp);
    *x = merge(cmp, a, b);
}

void merge_sort(queue_t *q, cmp_func cmp)
{
    _merge_sort(&q->head, q->size, cmp);
    list_ele_t *e = q->tail;
    while (e->next) {
        e = e->next;
    }
    q->tail = e;
}
