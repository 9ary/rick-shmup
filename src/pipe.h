#ifndef INCLUDE_PIPE_H
#define INCLUDE_PIPE_H

/*
 * FIFO/queue/ring buffer/whatever
 * Push void pointer in, get void pointer out, in order
 */

#include <SFML/System.h>

struct pipe_item
{
    void *data;
    struct pipe_item *next;
};

typedef struct pipe
{
    struct pipe_item *first;
    struct pipe_item *last;
    sfMutex *mutex;
} pipe_t;

pipe_t *pipe_create();
void pipe_free(pipe_t *pipe);

/*
 * Add element to the end of the queue
 */
void pipe_push(pipe_t *pipe, void *data);

/*
 * Retrieve the first element from the queue and remove it
 */
void *pipe_pop(pipe_t *pipe);

#endif
