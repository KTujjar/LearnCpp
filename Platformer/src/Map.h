#pragma once
#include "Global.h"
#include <SFML/Graphics.hpp>

class Map{
    public:
        Map();
        void createTileMap();
        void draw(sf::RenderWindow& window);
        bool checkCollision(float x, float y, float width, float height);

    
    private:
        sf::Texture floorTexture;
        sf::Texture wallTexture;
        sf::Sprite tile;
        std::vector<sf::Sprite> tileMap;
};