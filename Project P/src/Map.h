#pragma once
#include "Global.h"
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include "Texture.h"


class Map
{
private:
    
    bool drawTile(SDL_Renderer *r, SDL_Texture *t);

    struct Subset final
    {
        std::vector<SDL_Vertex> vertexData;
        SDL_Texture* texture = nullptr;
    };
    std::vector<Subset> m_subsets;

public:
    
    Map();
    void draw(SDL_Renderer *r) const;
    void load(SDL_Renderer *r, const char *filename);
    bool create(const tmx::Map &map, std::uint32_t layerIndex, const std::vector<std::unique_ptr<Texture>>& textures);
};
