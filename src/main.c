#include <stdlib.h>
#include <stdio.h>
#include <SFML/System.h>
#include "render.h"
#include "misc.h"
#include "version.h"
#include "statemachine.h"
#include "teststate/test.h"

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    sfThread *sm_thread;

    printf("%s r%s (%s-%s)\n", PROJ_NAME, git_rev, git_branch, git_commit);

    sm_thread = sfThread_create(&sm_loop, NULL);
    if (!sm_thread)
        return EXIT_FAILURE;
    sm_init();
    test_init();
    sm_push(&test_state);
    sfThread_launch(sm_thread);

    render_loop();

    sm_stop();
    sfThread_wait(sm_thread);
    sfThread_destroy(sm_thread);
    sm_cleanup();

    return EXIT_SUCCESS;
}
