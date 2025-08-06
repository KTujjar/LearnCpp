#include "Viewport.h"
Viewport::Viewport()
{

}

void Viewport::update(SDL_FRect player, int TILE_SIZE)
{
    v.mapViewport.x = ((player.x + TILE_SIZE / 2) - v.mapViewport.w / 2);
}