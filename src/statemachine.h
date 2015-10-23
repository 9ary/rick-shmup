#ifndef INCLUDE_STATEMACHINE_H
#define INCLUDE_STATEMACHINE_H

typedef struct
{
    void (*update)();
    void (*render)();
    void (*teardown)();
    int transparent;
} state_t;

void sm_init();
void sm_cleanup();
void sm_push(state_t *state);
void sm_pop();
void sm_stop();
void sm_loop();
void sm_render();

#endif
