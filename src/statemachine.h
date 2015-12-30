#ifndef INCLUDE_STATEMACHINE_H
#define INCLUDE_STATEMACHINE_H

/*
 * Context manager
 * Based on a state stack
 */

/*
 * State definition struct
 * Each state should provide one
 */
typedef struct
{
    /*
     * Called at 100Hz by the main loop
     */
    void (*update)(void *instance);

    /*
     * Called whenever the screen is redrawn if the state is active
     */
    void (*render)(void *instance);

    /*
     * Cleanup, called when popping a state
     */
    void (*teardown)(void *instance);

    /*
     * States can have multiple instances, let the functions know which one we
     * want
     */
    void *instance;

    /*
     * Whether to call the renderer of the underlying state
     */
    int transparent;
} state_t;

void sm_init();
void sm_cleanup();

/*
 * Start the state machine thread
 */
void sm_start();

/*
 * Push a new state onto the stack
 */
void sm_push(state_t *state);

/*
 * Pop the topmost state off the stack and clean up behind
 */
void sm_pop();

/*
 * Interrupt the machine
 */
void sm_stop();

/*
 * Calls the renderers from the stack following transparency
 * Used by the rendering thread
 */
void sm_render();

#endif
