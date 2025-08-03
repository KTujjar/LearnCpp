#pragma once
#include "Global.h"
#include <SDL3_image/SDL_image.h>

class Viewport
{
private:
    struct mapViewport{
        SDL_FRect mapViewport{
            0, 
            0,
            static_cast<float>(Global::windowWidth),
            static_cast<float>(Global::windowHeight)
        };
    };
public:
    mapViewport v;
    Viewport();
    void update(SDL_FRect player, int TILE_SIZE);
};
