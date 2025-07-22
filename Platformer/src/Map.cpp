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
    float x_interval = 0;
    for(int i = 0; i < 13; i++)
    {
        tile.setPosition({x_interval, HEIGHT - tile.getGlobalBounds().size.y});
        tileMap.push_back(tile);
        x_interval+=tile.getGlobalBounds().size.x;
    }
}

bool Map::checkCollision(float playerX, float playerY, float playerWidth, float playerHeight)
{
    for(int i = 0; i < tileMap.size(); i++)
    {
        if(playerY + playerHeight > HEIGHT - tile.getGlobalBounds().size.y)
        {
            return true;
        }
        return false;
    }
}

void Map::draw(sf::RenderWindow& window)
{
    //std::cout << tileMap.size() << std::endl;
    for(int i = 0; i < tileMap.size(); i++)
    {
        //std::cout << "here" << std::endl;
        window.draw(tileMap[i]);
    }
}