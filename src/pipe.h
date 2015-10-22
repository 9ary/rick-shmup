#ifndef INCLUDE_PIPE_H
#define INCLUDE_PIPE_H

struct pipe_item
{
    void *data;
    struct pipe_item *next;
};

typedef struct pipe
{
    struct pipe_item *first;
    struct pipe_item *last;
} pipe_t;

pipe_t *pipe_create();
void pipe_free(pipe_t *pipe);
void pipe_push(pipe_t *pipe, void *data);
void *pipe_pop(pipe_t *pipe);

#endif
