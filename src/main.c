#include <SFML/Window.h>
#include <SFML/Graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include "misc.h"

#define BUFFER_W 320
#define BUFFER_H 240

sfVector2f buffer_scale(sfVector2u size)
{
    float scale = min(size.x / (float) BUFFER_W, size.y / (float) BUFFER_H);
    sfVector2f ret = {scale, scale};
    return ret;
}

sfVector2f buffer_center(sfVector2u size)
{
    float scale = min(size.x / (float) BUFFER_W, size.y / (float) BUFFER_H);
    sfVector2f ret = {(size.x - BUFFER_W * scale) / 2,
                      (size.y - BUFFER_H * scale) / 2};
    return ret;
}

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    sfVideoMode mode = sfVideoMode_getDesktopMode();
    sfRenderWindow *window;
    sfRenderTexture *buffer;
    sfSprite *buffer_sprite;

    sfEvent event;
    sfVector2u size;

    window = sfRenderWindow_create(mode, PROJ_NAME,
                                   sfResize | sfClose | sfFullscreen, NULL);
    if (!window)
        return EXIT_FAILURE;

    buffer = sfRenderTexture_create(BUFFER_W, BUFFER_H, sfFalse);
    if (!buffer)
        return EXIT_FAILURE;

    buffer_sprite = sfSprite_create();
    if (!buffer_sprite)
        return EXIT_FAILURE;
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
                sfRenderWindow_close(window);

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
            }
        }

        sfRenderWindow_clear(window, sfBlack);

        sfRenderTexture_clear(buffer, sfWhite);
        sfRenderTexture_display(buffer);
        sfRenderWindow_drawSprite(window, buffer_sprite, NULL);

        sfRenderWindow_display(window);
    }

    sfSprite_destroy(buffer_sprite);
    sfRenderTexture_destroy(buffer);
    sfRenderWindow_destroy(window);

    return EXIT_SUCCESS;
}
