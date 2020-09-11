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


/*
 * Combine final list merge with restoration of standard doubly-linked
 * list structure.  This approach duplicates code from merge(), but
 * runs faster than the tidier alternatives of either a separate final
 * prev-link restoration pass, or maintaining the prev links
 * throughout.
 */
static inline void merge_final(cmp_func cmp,
                               queue_t *q,
                               list_ele_t *a,
                               list_ele_t *b)
{
    list_ele_t head = {.next = q->head, .value = ""};
    list_ele_t *tail = &head;
    // size_t count = 0;

    for (;;) {
        /* if equal, take 'a' -- important for sort stability */
        if (cmp(&a->value, &b->value) <= 0) {
            tail->next = a;
            // a->prev = tail;
            tail = a;
            a = a->next;
            if (!a)
                break;
        } else {
            tail->next = b;
            // b->prev = tail;
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
        /*
         * If the merge is highly unbalanced (e.g. the input is
         * already sorted), this loop may run many iterations.
         * Continue callbacks to the client even though no
         * element comparison is needed, so the client's cmp()
         * routine can invoke cond_resched() periodically.
         */
        // if (unlikely(!++count))
        //      cmp(b, b);
        // b->prev = tail;
        tail = b;
        b = b->next;
    } while (b);

    /* And the final links to make a circular doubly-linked list */
    // tail->next = head;
    // head->prev = tail;
    q->head = head.next;
    q->tail = tail;
}

static list_ele_t *_merge_sort(list_ele_t *head, size_t size, cmp_func cmp)
{
    RETURN_IF_NULL(head && head->next, head);

    size_t count = size >> 1;
    list_ele_t *b = head;
    for (size_t i = 1; i < count; i++)
        b = b->next;

    list_ele_t *a = b->next;
    b->next = NULL;

    return merge(cmp, _merge_sort(a, count + (size & 1), cmp),
                 _merge_sort(head, count, cmp));
}

// cppcheck-suppress unusedFunction
void merge_sort(queue_t *q, cmp_func cmp)
{
    size_t count = q->size >> 1;
    list_ele_t *b = q->head;
    for (size_t i = 1; i < count; i++)
        b = b->next;

    list_ele_t *a = b->next;
    b->next = NULL;

    merge_final(cmp, q, _merge_sort(a, count + (q->size & 1), cmp),
                _merge_sort(q->head, count, cmp));
}
