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

    printf("%s r%s (%s-%s)\n", PROJ_NAME, git_rev, git_branch, git_commit);

    sm_init();
    sm_push(test_init());
    sm_start();

    render_loop();

    sm_stop();
    sm_cleanup();

    return EXIT_SUCCESS;
}
