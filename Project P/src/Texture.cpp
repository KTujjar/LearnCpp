#include "Texture.h"


Texture::Texture()
        : texture(nullptr)
{
    
}

bool Texture::loadTexture(SDL_Renderer *r, const char *filename)
{
    assert(r != nullptr);
    assert(filename != nullptr);

    texture = IMG_LoadTexture(r, filename);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    if(texture != nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }

}

SDL_Point Texture::getSize() const
{
    SDL_Point temp;

    if(texture != nullptr)
    {
        temp.x = texture->w;
        temp.y = texture->h;
        return temp;
    }
    return temp;
}