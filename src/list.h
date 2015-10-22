#ifndef INCLUDE_LIST_H
#define INCLUDE_LIST_H

typedef struct
{
    int length;
    void **items;
} list_t;

list_t *list_create();
void list_free(list_t *list);
void list_add(list_t *list, void *item);
void list_pop(list_t *list);

#endif
