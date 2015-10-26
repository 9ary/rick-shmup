#include <stdlib.h>
#include <SFML/System.h>
#include "statemachine.h"
#include "list.h"
#include "pipe.h"

typedef enum
{
    cmd_push,
    cmd_pop,
    cmd_stop
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

void sm_stop()
{
    sm_cmd_push(cmd_stop, NULL);
}

void sm_loop()
{
    int cur_state = sm_stack->length - 1;
    cmd_t *cmd;

    sfClock *clock = sfClock_create();
    if (!clock)
        exit(EXIT_FAILURE);
    const sfTime time_dt = sfSeconds(0.01);
    sfTime time_acc = time_dt;
    sfTime time_now = sfTime_Zero, time_next;

    while (1)
    {
        time_next = sfClock_getElapsedTime(clock);
        time_acc.microseconds += time_next.microseconds - time_now.microseconds;
        time_now = time_next;

        while (time_acc.microseconds >= time_dt.microseconds)
        {
            while ((cmd = pipe_pop(sm_cmds)))
            {
                switch (cmd->id)
                {
                    case cmd_push:
                        list_add(sm_stack, cmd->args);
                        cur_state++;
                        free(cmd);
                        continue;

                    case cmd_pop:
                        ((state_t *) sm_stack->items[cur_state])->teardown();
                        list_pop(sm_stack);
                        cur_state--;
                        free(cmd);
                        continue;

                    case cmd_stop:
                        free(cmd);
                        sfClock_destroy(clock);
                        return;
                }
            }

            ((state_t *) sm_stack->items[cur_state])->update();

            time_acc.microseconds -= time_dt.microseconds;
        }

        sfTime time_sleep = {time_dt.microseconds - time_acc.microseconds};
        sfSleep(time_sleep);
    }
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
