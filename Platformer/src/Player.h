#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    Player();

    void update(float deltaTime);
    void handleInput();
    void draw(sf::RenderWindow& window);

private:
    sf::Sprite playerSprite;
    sf::Texture idleTexture;
    sf::Texture walkTexture;

    unsigned int texWidth = 0;
    float elapsedTimer = 0.f;
    float frameDelay = 0.15f;

    void animate();
    void moveLeft();
    void moveRight();
};