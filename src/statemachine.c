#include <stdlib.h>
#include "statemachine.h"
#include "list.h"
#include "pipe.h"

typedef enum
{
    cmd_push,
    cmd_pop
} cmd_id_t;

typedef struct
{
    cmd_id_t id;
    void *args;
} cmd_t;

static list_t *sm_stack;
static pipe_t *sm_cmds;

static void sm_cmd_push(cmd_id_t id, void *args)
{
    cmd_t *cmd = malloc(sizeof(cmd_t));
    cmd->id = id;
    cmd->args = args;
    pipe_push(sm_cmds, cmd);
}

void sm_init()
{
    sm_stack = list_create();
    sm_cmds = pipe_create();
}

void sm_cleanup()
{
    for (int i = 0; i < sm_stack->length; i++)
        ((state_t *) sm_stack->items[i])->teardown();
    list_free(sm_stack);
    sm_stack = NULL;
    pipe_free(sm_cmds);
    sm_cmds = NULL;
}

void sm_push(state_t *state)
{
    sm_cmd_push(cmd_push, state);
}

void sm_pop()
{
    sm_cmd_push(cmd_pop, NULL);
}

void sm_loop()
{
    int cur_state = sm_stack->length - 1;
    cmd_t *cmd;
    while ((cmd = pipe_pop(sm_cmds)))
    {
        switch (cmd->id)
        {
            case cmd_push:
                list_add(sm_stack, cmd->args);
                cur_state++;
                continue;

            case cmd_pop:
                ((state_t *) sm_stack->items[cur_state])->teardown();
                list_pop(sm_stack);
                cur_state--;
                continue;
        }
    }

    ((state_t *) sm_stack->items[cur_state])->update();
}

void sm_render()
{
    int cur_state = sm_stack->length - 1;

    // Find the lowest opaque state
    while (((state_t *) sm_stack->items[cur_state])->transparent)
        cur_state--;

    // Render from bottom to top
    while (cur_state < sm_stack->length)
    {
        ((state_t *) sm_stack->items[cur_state])->render();
        cur_state++;
    }
}
