#include <stdio.h>
#include <SFML/Graphics.h>
#include "test.h"
#include "render.h"
#include "input.h"

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
    if (input_key_pressed(input_up))
        printf("Up pressed\n");

    if (input_key_held(input_up))
        printf("Up held\n");

    if (input_key_released(input_up))
        printf("Up released\n");
}

static void test_render()
{
    sfRenderTexture_drawSprite(render_buffer, dawn_sprite, NULL);
}

static void test_teardown()
{
    sfTexture_destroy(dawn);
}

state_t test_state =
{
    .update = &test_update,
    .render = &test_render,
    .teardown = &test_teardown,
    .transparent = 0
};
