#include <stdlib.h>
#include "list.h"

list_t *list_create()
{
    list_t *list = malloc(sizeof(list_t));
    list->length = 0;
    list->items = NULL;

    return list;
}

void list_free(list_t *list)
{
    if (list)
    {
        free(list->items);
        free(list);
    }
}

static void list_realloc(list_t *list)
{
    list->items = realloc(list->items, sizeof(void *) * list->length);
}

void list_add(list_t *list, void *item)
{
    list->length++;
    list_realloc(list);
    list->items[list->length - 1] = item;
}

void list_pop(list_t *list)
{
    list->length--;
    list_realloc(list);
}
