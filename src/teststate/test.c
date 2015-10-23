#include <SFML/System.h>
#include <SFML/Graphics.h>
#include "test.h"
#include "render.h"

static sfTexture *dawn;
static sfSprite *dawn_sprite;

void test_init()
{
    dawn = sfTexture_createFromFile("art/dawn.png", NULL);
    dawn_sprite = sfSprite_create();
    sfSprite_setTexture(dawn_sprite, dawn, sfTrue);
}

static void test_update()
{
    sfTime time = {100 * 1000};
    sfSleep(time);
}

static void test_render()
{
    sfRenderTexture_drawSprite(render_buffer, dawn_sprite, NULL);
}

static void test_teardown()
{
    sfTexture_destroy(dawn);
}

state_t test_state = {.update = &test_update,
                      .render = &test_render,
                      .teardown = &test_teardown,
                      .transparent = 0};
