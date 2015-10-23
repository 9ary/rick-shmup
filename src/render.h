#ifndef INCLUDE_RENDER_H
#define INCLUDE_RENDER_H

#include <SFML/Graphics.h>

#define BUFFER_W 320
#define BUFFER_H 240

extern sfRenderTexture *render_buffer;

void render_loop();

#endif
