#pragma once
#include "Global.h"
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <tmxlite/Map.hpp>
#include <cassert>

class Texture
{
private:
    
public:
    Texture();
    SDL_Texture *texture;

    operator SDL_Texture*() const {
        return texture;
    }

    bool loadTexture(SDL_Renderer *r, const char *filename);
    SDL_Point getSize() const;
};
