#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "merge_sort.h"
#include "queue.h"

static list_ele_t *new_ele_str(char const *s)
{
    list_ele_t *e = malloc(sizeof(list_ele_t));
    RETURN_IF_NULL(e, NULL);

    size_t length = strlen(s) + 1;
    e->value = malloc(length);
    if (!e->value) {
        free(e);
        return NULL;
    } else {
        STRNCPY(e->value, s, length);
        return e;
    }
}

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    RETURN_IF_NULL(q, NULL);

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    RETURN_IF_NULL(q, );

    /* Free queue structure */
    while (q->head) {
        free(q->head->value);
        list_ele_t *p = q->head->next;
        free(q->head);
        q->head = p;
    }

    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, const char *s)
{
    RETURN_IF_NULL(q, false);

    list_ele_t *e = new_ele_str(s);
    RETURN_IF_NULL(e, false);

    e->next = q->head;

    q->head = e;
    if (!(q->size++))
        q->tail = e;

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, const char *s)
{
    RETURN_IF_NULL(q, false);

    RETURN_IF_NULL(q->head, q_insert_head(q, s));

    list_ele_t *e = new_ele_str(s);
    RETURN_IF_NULL(e, false);

    e->next = NULL;

    q->tail->next = e;
    q->tail = e;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    RETURN_IF_NULL(q && q->size, false);

    list_ele_t *e = q->head;
    q->head = e->next;
    if (sp && bufsize)
        STRNCPY(sp, e->value, bufsize);

    free(e->value);
    free(e);
    if (!(--q->size))
        q->tail = NULL;

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(const queue_t *q)
{
    RETURN_IF_NULL(q, 0);

    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    RETURN_IF_NULL(q && 1 < q->size, );

    q->tail = q->head;
    list_ele_t *e = q->head->next;
    while (e->next) {
        list_ele_t *t = e->next;
        e->next = q->head;
        q->head = e;
        e = t;
    }
    e->next = q->head;
    q->head = e;
    q->tail->next = NULL;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q, const cmp_func cmp)
{
    RETURN_IF_NULL(q && 1 < q->size, );

    merge_sort(q, cmp);
}
