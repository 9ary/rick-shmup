#ifndef INCLUDE_LIST_H
#define INCLUDE_LIST_H

/*
 * Dynamically sized array of void pointers
 */

typedef struct
{
    int length;
    void **items;
} list_t;

list_t *list_create();
void list_free(list_t *list);

/*
 * Add one item to the end of the list
 */
void list_add(list_t *list, void *item);

/*
 * Remove the last item from the list
 */
void list_pop(list_t *list);

#endif
