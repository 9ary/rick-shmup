#include <stdlib.h>
#include <SFML/Window.h>
#include <SFML/Graphics.h>
#include "render.h"
#include "misc.h"

static sfVector2f buffer_scale(sfVector2u size)
{
    float scale = min(size.x / (float) BUFFER_W, size.y / (float) BUFFER_H);
    sfVector2f ret = {scale, scale};
    return ret;
}

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
    sfRenderWindow *window;
    sfRenderTexture *buffer;
    sfSprite *buffer_sprite;

    sfEvent event;
    sfVector2u size;

    window = sfRenderWindow_create(mode, PROJ_NAME,
                                   sfResize | sfClose | sfFullscreen, NULL);
    if (!window)
        exit(EXIT_FAILURE);

    buffer = sfRenderTexture_create(BUFFER_W, BUFFER_H, sfFalse);
    if (!buffer)
        exit(EXIT_FAILURE);

    buffer_sprite = sfSprite_create();
    if (!buffer_sprite)
        exit(EXIT_FAILURE);
    sfSprite_setTexture(buffer_sprite, sfRenderTexture_getTexture(buffer),
                        sfTrue);
    size = sfRenderWindow_getSize(window);
    sfSprite_setScale(buffer_sprite, buffer_scale(size));
    sfSprite_setPosition(buffer_sprite, buffer_center(size));

    while (sfRenderWindow_isOpen(window))
    {
        // Process events
        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed)
            {
                sfRenderWindow_close(window);
                continue;
            }

            if (event.type == sfEvtResized)
            {
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
            }
        }

        sfRenderWindow_clear(window, sfBlack);
        sfRenderTexture_clear(buffer, sfBlack);

        // TODO Call the current state's renderer

        sfRenderTexture_display(buffer);
        sfRenderWindow_drawSprite(window, buffer_sprite, NULL);
        sfRenderWindow_display(window);
    }

    sfSprite_destroy(buffer_sprite);
    sfRenderTexture_destroy(buffer);
    sfRenderWindow_destroy(window);
}
