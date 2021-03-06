#ifndef LAB0_QUEUE_H
#define LAB0_QUEUE_H

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */
#include <stdbool.h>
#include <stddef.h>
#include "str_cmp.h"

#ifndef likely
#define likely(x) __builtin_expect(!!(x), 1)
#endif
#ifndef unlikely
#define unlikely(x) __builtin_expect((x), 0)
#endif

#define RETURN_IF_NULL(obj, ret_val) \
    do {                             \
        if (unlikely(!(obj)))        \
            return ret_val;          \
    } while (0)

#define STRNCPY(dest, src, len)  \
    do {                         \
        strncpy(dest, src, len); \
        dest[len - 1] = '\0';    \
    } while (0)

/* Data structure declarations */

/* Linked list element */
typedef struct ELE {
    /* Pointer to array holding string.
     * This array needs to be explicitly allocated and freed
     */
    char *value;
    struct ELE *next;
} list_ele_t;

/* Queue structure */
typedef struct {
    list_ele_t *head; /* Linked list of elements */
    list_ele_t *tail; /* Element at tail of queue */
    size_t size;
} queue_t;

/* Operations on queue */

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new();

/*
 * Free ALL storage used by queue.
 * No effect if q is NULL
 */
void q_free(queue_t *q);

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, const char *s);

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, const char *s);

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize);

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(const queue_t *q);

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q);

/*
 * Sort elements of queue in specify order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q, const cmp_func cmp);

#endif /* LAB0_QUEUE_H */
