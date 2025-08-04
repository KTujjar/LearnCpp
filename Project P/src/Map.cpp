#include "Map.h"

Map::Map()
{

}

//loads the tileMap and tileSets
void Map::load(SDL_Renderer *r, const char *filename, SDL_FRect viewport)
{
    std::vector<std::unique_ptr<Texture>> textures;
    std::vector<std::unique_ptr<Map>> renderLayers;
    tmx::Map map;
    if(map.load(filename))
    {
        const auto& tileSets = map.getTilesets();
        assert(!tileSets.empty());
        for (const auto& ts : tileSets)
        {
            textures.emplace_back(std::make_unique<Texture>());
            if (!textures.back()->loadTexture(r, ts.getImagePath().c_str()))
            {
                SDL_Log("Failed to load texture: %s", ts.getImagePath().c_str());
            }
        }

        //load the layers
        const auto& mapLayers = map.getLayers();
        for (auto i = 0u; i < mapLayers.size(); ++i)
        {
            if (mapLayers[i]->getType() == tmx::Layer::Type::Tile)
            {
                create(map, i, textures, viewport); //just cos we're using C++14
            }
        }
    
    }
}

//Loads every tile before drawing
bool Map::create(const tmx::Map &map, std::uint32_t layerIndex, const std::vector<std::unique_ptr<Texture>>& textures, SDL_FRect viewport)
{
    const auto& layers = map.getLayers();
    assert(layers[layerIndex]->getType() == tmx::Layer::Type::Tile);
    
    const auto& layer = layers[layerIndex]->getLayerAs<tmx::TileLayer>();
    const auto mapSize = map.getTileCount();
    const auto mapTileSize = map.getTileSize();
    const auto& tileSets = map.getTilesets();

    const auto tintColour = layer.getTintColour();
    const SDL_FColor vertColour =
    {
        tintColour.r / 255.f,
        tintColour.g / 255.f,
        tintColour.b / 255.f,
        tintColour.a / 255.f
    };

    for (auto i = 0u; i < tileSets.size(); ++i)
    {
        //check tile ID to see if it falls within the current tile set
        const auto& ts = tileSets[i];
        const auto& tileIDs = layer.getTiles();

        const auto texSize = textures[i]->getSize();
        const auto tileCountX = texSize.x / mapTileSize.x;
        const auto tileCountY = texSize.y / mapTileSize.y;

        const float uNorm = static_cast<float>(mapTileSize.x) / texSize.x;
        const float vNorm = static_cast<float>(mapTileSize.y) / texSize.y;

        std::vector<SDL_Vertex> verts;
        for (auto y = 0u; y < mapSize.y; ++y)
        {
            for (auto x = 0u; x < mapSize.x; ++x)
            {
                const auto idx = y * mapSize.x + x;
                if (idx < tileIDs.size() && tileIDs[idx].ID >= ts.getFirstGID()
                    && tileIDs[idx].ID < (ts.getFirstGID() + ts.getTileCount()))
                {
                    //Collision Rects
                    SDL_FRect tileRect = {
                        static_cast<float>(x * mapTileSize.x),
                        static_cast<float>(y * mapTileSize.y),
                        static_cast<float>(mapTileSize.x),
                        static_cast<float>(mapTileSize.y)
                    };
                    
                    solidRects.push_back(tileRect);
                    
                    
                    //tex coords
                    auto idIndex = (tileIDs[idx].ID - ts.getFirstGID());
                    float u = static_cast<float>(idIndex % tileCountX);
                    float v = static_cast<float>(idIndex / tileCountX);
                    u *= mapTileSize.x; //TODO we should be using the tile set size, as this may be different from the map's grid size
                    v *= mapTileSize.y;

                    //normalise the UV
                    u /= textures[i]->getSize().x;
                    v /= textures[i]->getSize().y;

                    //vert pos
                    const float tilePosX = static_cast<float>(x) * mapTileSize.x + viewport.x;
                    //SDL_Log("%f", viewport.x);
                    const float tilePosY = (static_cast<float>(y) * mapTileSize.y);


                    //push back to vert array


                    //A
                    SDL_Vertex vert = { { tilePosX, tilePosY }, vertColour, {u, v} };
                    verts.emplace_back(vert);
                    
                    //B
                    vert = { { tilePosX + mapTileSize.x, tilePosY }, vertColour, {u + uNorm, v} };
                    verts.emplace_back(vert);

                    //C
                    vert = { { tilePosX, tilePosY + mapTileSize.y}, vertColour, {u, v + vNorm} };
                    verts.emplace_back(vert);
                    
                    //C
                    vert = { { tilePosX, tilePosY + mapTileSize.y}, vertColour, {u, v + vNorm} };
                    verts.emplace_back(vert);

                    //B
                    vert = { { tilePosX + mapTileSize.x, tilePosY }, vertColour, {u + uNorm, v} };
                    verts.emplace_back(vert);

                    //D
                    vert = { { tilePosX + mapTileSize.x, tilePosY + mapTileSize.y }, vertColour, {u + uNorm, v + vNorm} };
                    verts.emplace_back(vert);
                }
            }
        }

        if (!verts.empty())
        {
            m_subsets.emplace_back();
            m_subsets.back().texture = textures[i]->texture;
            m_subsets.back().vertexData.swap(verts);
        }
    }

    return true;
}

//Draws triangles to create the tiles from the given vertices.
void Map::draw(SDL_Renderer *r, SDL_FRect viewport) const
{
    for (const auto& s : m_subsets)
    {

        std::vector<SDL_Vertex> adjustedVerts = s.vertexData; // copy original

        // Apply the inverse of camera transform
        for (auto& v : adjustedVerts)
        {
            v.position.x -= viewport.x;
            v.position.y -= viewport.y;
        }
        //Creates 2 triangles per tile to make the full square
        SDL_RenderGeometry(r, s.texture, adjustedVerts.data(), static_cast<std::int32_t>(s.vertexData.size()), nullptr, 0);
    }
}