#include <stdlib.h>
#include "pipe.h"

pipe_t *pipe_create()
{
    pipe_t *pipe = malloc(sizeof(pipe_t));
    pipe->first = NULL;
    pipe->last = NULL;
    return pipe;
}

void pipe_free(pipe_t *pipe)
{
    while (pipe_pop(pipe))
        ;
    free(pipe);
}

void pipe_push(pipe_t *pipe, void *data)
{
    struct pipe_item *next = malloc(sizeof(struct pipe_item));
    next->data = data;
    next->next = NULL;

    if (pipe->last)
        pipe->last->next = next;
    else
        pipe->first = next;
    pipe->last = next;
}

void *pipe_pop(pipe_t *pipe)
{
    if (pipe->first)
    {
        void *ret = pipe->first->data;
        struct pipe_item *next = pipe->first->next;
        free(pipe->first);
        pipe->first = next;
        if (!next)
            pipe->last = NULL;
        return ret;
    }
    else
        return NULL;
}
