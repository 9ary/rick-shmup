#include <stdlib.h>
#include <SFML/Window.h>
#include <SFML/Graphics.h>
#include "render.h"
#include "statemachine.h"
#include "input.h"
#include "misc.h"

sfRenderTexture *render_buffer;
sfRenderWindow *window;

/*
 * Returns the scaling factor given the window size
 */
static sfVector2f buffer_scale(sfVector2u size)
{
    float scale = min(size.x / (float) BUFFER_W, size.y / (float) BUFFER_H);
    sfVector2f ret = {scale, scale};
    return ret;
}

/*
 * Returns the position of the position of the buffer on the window
 */
static sfVector2f buffer_center(sfVector2u size)
{
    float scale = min(size.x / (float) BUFFER_W, size.y / (float) BUFFER_H);
    sfVector2f ret = {(size.x - BUFFER_W * scale) / 2,
                      (size.y - BUFFER_H * scale) / 2};
    return ret;
}

void render_loop()
{
    sfVideoMode mode = sfVideoMode_getDesktopMode();
    sfSprite *buffer_sprite;

    sfEvent event;
    sfVector2u size;

    // TODO make window settings configurable
    window = sfRenderWindow_create(mode, PROJ_NAME,
                                   sfResize | sfClose | sfFullscreen, NULL);
    sfRenderWindow_setFramerateLimit(window, 60);

    render_buffer = sfRenderTexture_create(BUFFER_W, BUFFER_H, sfFalse);

    buffer_sprite = sfSprite_create();
    sfSprite_setTexture(buffer_sprite,
                        sfRenderTexture_getTexture(render_buffer), sfTrue);
    size = sfRenderWindow_getSize(window);
    sfSprite_setScale(buffer_sprite, buffer_scale(size));
    sfSprite_setPosition(buffer_sprite, buffer_center(size));

    while (sfRenderWindow_isOpen(window))
    {
        // Process events
        while (sfRenderWindow_pollEvent(window, &event))
        {
            switch (event.type)
            {
                case sfEvtClosed:
                    sfRenderWindow_close(window);
                    continue;

                case sfEvtResized:
                    // Adapt the window's surface and recalculate the buffer
                    // scale on window resize
                    size = sfRenderWindow_getSize(window);
                    sfFloatRect rect = {0, 0, size.x, size.y};
                    sfView *view = sfView_createFromRect(rect);
                    if (view)
                    {
                        sfRenderWindow_setView(window, view);
                        sfView_destroy(view);
                    }

                    sfSprite_setScale(buffer_sprite, buffer_scale(size));
                    sfSprite_setPosition(buffer_sprite, buffer_center(size));
                    continue;

                default:
                    continue;
            }
        }

        input_poll();

        sfRenderWindow_clear(window, sfBlack);
        sfRenderTexture_clear(render_buffer, sfBlack);

        // Ask the state machine to call the state renderers
        sm_render();

        // Then shove it onto the user's face
        sfRenderTexture_display(render_buffer);
        sfRenderWindow_drawSprite(window, buffer_sprite, NULL);
        sfRenderWindow_display(window);
    }

    sfSprite_destroy(buffer_sprite);
    sfRenderTexture_destroy(render_buffer);
    sfRenderWindow_destroy(window);
}
