#pragma once
#include <SFML/Graphics.hpp>
#include "Global.h"
#include "Map.h"

class Player {
public:
    Player();

    void update(float deltaTime);
    void handleInput();
    void draw(sf::RenderWindow& window);
    void collision();

private:
    Map map;

    sf::Sprite playerSprite;
    sf::Texture idleTexture;
    sf::Texture walkTexture;

    unsigned int texWidth = 0;
    float verticalVelocity = 0;
    float gravity = 0.1;
    float elapsedTimer = 0.f;
    float frameDelay = 0.15f;

    void animate();
    void moveLeft();
    void moveRight();
};