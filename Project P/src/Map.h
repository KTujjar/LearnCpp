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
    int TILE_SIZE = 16;
    Map();
    std::vector<SDL_FRect> solidRects;
    void draw(SDL_Renderer *r, SDL_FRect viewport) const;
    void load(SDL_Renderer *r, const char *filename, SDL_FRect viewport);
    bool create(const tmx::Map &map, std::uint32_t layerIndex, const std::vector<std::unique_ptr<Texture>>& textures, SDL_FRect viewport);
};
