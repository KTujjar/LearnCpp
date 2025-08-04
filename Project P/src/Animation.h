#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include "Global.h"
#include "Texture.h"

class Animation
{
private:

public:
    std::vector<SDL_FRect> frames;
    Texture texture;
    Animation();
    bool load(float frame_width, float frame_height);
};
