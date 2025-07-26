#pragma once
#include "Global.h"
#include <SFML/Graphics.hpp>
#include "SFMLOrthogonalLayer.hpp"

class Map{
    public:
        Map();
        void createTileMap();
        void draw(sf::RenderWindow& window);
        void collisionRect();
        std::vector<sf::FloatRect> getCollisionRect();

    
    private:
        std::vector<sf::FloatRect> collisionRects;
        sf::Texture floorTexture;
        sf::Texture wallTexture;
        sf::Sprite tile;
        tmx::Map map;
        std::vector<sf::Sprite> tileMap;
};