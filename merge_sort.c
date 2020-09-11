// SPDX-License-Identifier: GPL-2.0
#include "queue.h"

// From https://github.com/torvalds/linux/blob/master/lib/list_sort.c
/*
 * Returns a list organized in an intermediate format suited
 * to chaining of merge() calls: null-terminated, no reserved or
 * sentinel head node.
 */
static inline list_ele_t *merge(const cmp_func cmp,
                                list_ele_t *a,
                                list_ele_t *b)
{
    // cppcheck-suppress unassignedVariable
    list_ele_t *head, **tail = &head;

    for (;;) {
        if (cmp(a->value, b->value) <= 0) {
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

/*
 * Combine final list merge with restoration
 */
static inline void merge_final(const cmp_func cmp,
                               queue_t *q,
                               list_ele_t *a,
                               list_ele_t *b)
{
    list_ele_t head = {.next = q->head};
    list_ele_t *tail = &head;

    for (;;) {
        if (cmp(a->value, b->value) <= 0) {
            tail->next = a;
            tail = a;
            a = a->next;
            if (!a)
                break;
        } else {
            tail->next = b;
            tail = b;
            b = b->next;
            if (!b) {
                b = a;
                break;
            }
        }
    }

    /* Finish linking remainder of list b on to tail */
    tail->next = b;
    do {
        tail = b;
        b = b->next;
    } while (b);

    q->head = head.next;
    q->tail = tail;
}

void merge_sort(queue_t *q, const cmp_func cmp)
{
    list_ele_t *sorted[64];
    size_t sorted_count = 0;

    list_ele_t *list = q->head->next, *pending = q->head;
    pending->next = NULL;
    size_t count = 1;

    do {
        size_t bits;
        sorted[sorted_count] = pending;

        size_t prev_count = 0;
        /* Find the least-significant clear bit in count */
        for (bits = count; bits & 1; bits >>= 1)
            prev_count++;

        /* Do the indicated merge */
        if (likely(bits)) {
            size_t end = sorted_count - prev_count;
            size_t start = end - 1;

            sorted[start] = merge(cmp, sorted[start], sorted[end]);

            for (size_t i = end; i < sorted_count; i++)
                sorted[i] = sorted[i + 1];

        } else {
            sorted_count++;
        }

        pending = list;
        list = list->next;
        pending->next = NULL;
        count++;
    } while (list);

    for (size_t i = sorted_count - 1; i > 0; i--) {
        pending = merge(cmp, sorted[i], pending);
    }

    merge_final(cmp, q, sorted[0], pending);
}
