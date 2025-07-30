#include "Global.h"
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <tmxlite/Map.hpp>

class Map
{
private:
    
    void drawTile(SDL_Renderer *r, SDL_Texture *t);
public:
    
    Map();
    void drawMap(SDL_Renderer *r, SDL_Texture *t);
};
