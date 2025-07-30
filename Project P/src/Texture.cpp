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

    if(texture != nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }

}
