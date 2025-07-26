#include "Map.h"
#include <iostream>


Map::Map()
    : tile(floorTexture)
{
    sf::IntRect grass = {{32, 464}, {16,16}};
    floorTexture.loadFromFile("../assets/Tiles/Tiles.png");
    tile.setTextureRect(grass);
    tile.setScale({4.f, 4.f});
    createTileMap();
}

void Map::createTileMap()
{
    map.load("../assets/Tiles/level3.tmx");
    collisionRect();
}

//creates collisionRect
void Map::collisionRect()
{
    const auto& layers = map.getLayers();
    for (const auto& layerPtr : layers) {
        if (layerPtr->getType() != tmx::Layer::Type::Tile) continue;

        const auto& tileLayer = layerPtr->getLayerAs<tmx::TileLayer>();
        const auto& tiles = tileLayer.getTiles(); // vector of tile structs {ID, flip flags} :contentReference[oaicite:5]{index=5}
        unsigned mapWidth = map.getTileCount().x;
        unsigned tileW    = map.getTileSize().x;
        unsigned tileH    = map.getTileSize().y;

        for (size_t i = 0; i < tiles.size(); ++i) {
            const auto& tile = tiles[i];
            if (tile.ID == 0) continue; // no tile here

            unsigned x = i % mapWidth;
            unsigned y = i / mapWidth;

            sf::FloatRect rect{
                {float(x * tileW),
                float(y * tileH)},
                {float(tileW),
                float(tileH)}
            };

            collisionRects.push_back(rect);
        }
    }
}

std::vector<sf::FloatRect> Map::getCollisionRect()
{
    return collisionRects;
}

void Map::draw(sf::RenderWindow& window)
{
    // //std::cout << tileMap.size() << std::endl;
    // for(int i = 0; i < tileMap.size(); i++)
    // {
    //     //std::cout << "here" << std::endl;
    //     window.draw(tileMap[i]);
    // }

    
    MapLayer layerZero(map, 0);

    window.draw(layerZero);

}