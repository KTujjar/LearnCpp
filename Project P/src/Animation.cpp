#include "Animation.h"

Animation::Animation()
{
    
}

//load the animation given the width and height of canvas for each frame
bool Animation::load(float frame_width, float frame_height)
{
    frames.clear();
    SDL_FRect t;
    float texWidth = 0, texHeight = 0;
    if (!SDL_GetTextureSize(texture, &texWidth, &texHeight)) 
    {
        SDL_Log("Failed to get texture size: %s", SDL_GetError());
        return false;
    }
    
    int frameCount = static_cast<int>(texWidth / frame_width);

    if(frameCount <= 0)
    {
        SDL_Log("Invalid frameCount = %d", frameCount);
        return false;
    }
    for(int i = 0; i < frameCount; i++)
    {
        t = {0 + (frame_width * i), 0, (float)frame_height, (float)frame_height};
        //SDL_Log("%f", t.x);
        frames.push_back(t);
    }

    return true;
}