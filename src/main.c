#include <stdlib.h>
#include <stdio.h>
#include "render.h"
#include "misc.h"
#include "version.h"

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    printf("%s r%s (%s-%s)\n", PROJ_NAME, git_rev, git_branch, git_commit);
    render_loop();

    return EXIT_SUCCESS;
}
