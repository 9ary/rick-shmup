#include <stdlib.h>
#include "render.h"
#include "misc.h"

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    render_loop();

    return EXIT_SUCCESS;
}
