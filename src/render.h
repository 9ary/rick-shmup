#ifndef INCLUDE_RENDER_H
#define INCLUDE_RENDER_H

/*
 * Main rendering thread
 */

#include <SFML/Graphics.h>

/*
 * Rendering surface resolution
 */
#define BUFFER_W 320
#define BUFFER_H 240

/*
 * Main render surface, everything that draws should do it on there
 */
extern sfRenderTexture *render_buffer;

void render_loop();

#endif
