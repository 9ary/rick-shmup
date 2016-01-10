#include <stdlib.h>
#include <SFML/System.h>
#include "statemachine.h"
#include "list.h"
#include "pipe.h"
#include "input.h"

/*
 * Shortcut to access the nth state
 */
#define STATE(n) ((state_t *) sm_stack->items[n])

/*
 * Internal commands
 */
typedef enum
{
    cmd_push,
    cmd_pop,
    cmd_stop
} cmd_id_t;

/*
 * Command struct to store into the pipe
 */
typedef struct
{
    cmd_id_t id;
    void *args;
} cmd_t;

/*
 * The actual stack
 */
static list_t *sm_stack = NULL;

/*
 * Command pipe
 */
static pipe_t *sm_cmds = NULL;

static sfThread *sm_thread = NULL;

static void sm_loop();

/*
 * Command wrapper
 */
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
    sm_thread = sfThread_create(&sm_loop, NULL);
    if (!sm_thread)
        exit(EXIT_FAILURE);
}

void sm_cleanup()
{
    for (int i = 0; i < sm_stack->length; i++)
        STATE(i)->teardown(STATE(i)->instance);
    list_free(sm_stack);
    sm_stack = NULL;
    pipe_free(sm_cmds);
    sm_cmds = NULL;
    sfThread_destroy(sm_thread);
    sm_thread = NULL;
}

void sm_start()
{
    sfThread_launch(sm_thread);
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
    sfThread_wait(sm_thread);
}

static void sm_loop()
{
    // Current state is always the topmost element of the stack
    int cur_state = sm_stack->length - 1;

    // Used to receive commands
    cmd_t *cmd;

    sfClock *clock = sfClock_create();

    // Update tick duration
    const sfTime time_dt = sfSeconds(0.01);
    // Time we've been left behind
    sfTime time_acc = time_dt;
    sfTime time_now = sfTime_Zero, time_next;

    while (1)
    {
        // Calculate the time the last tick took
        time_next = sfClock_getElapsedTime(clock);
        time_acc.microseconds += time_next.microseconds - time_now.microseconds;
        time_now = time_next;

        // Catch up when we're late
        while (time_acc.microseconds >= time_dt.microseconds)
        {
            // Process commands
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
                        STATE(cur_state)->teardown(STATE(cur_state)->instance);
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

            input_copy();

            // Call the current state's update routine
            STATE(cur_state)->update(STATE(cur_state)->instance);

            // Simulation has stepped, update the time accumulator
            time_acc.microseconds -= time_dt.microseconds;
        }

        // Put the thread to sleep for the rest of the tick
        sfTime time_sleep = {time_dt.microseconds - time_acc.microseconds};
        sfSleep(time_sleep);
    }
}

void sm_render()
{
    // Current state is always the topmost element of the stack
    int cur_state = sm_stack->length - 1;

    // Find the lowest opaque state
    while (STATE(cur_state)->transparent)
        cur_state--;

    // Render from bottom to top
    while (cur_state < sm_stack->length)
    {
        STATE(cur_state)->render(STATE(cur_state)->instance);
        cur_state++;
    }
}
