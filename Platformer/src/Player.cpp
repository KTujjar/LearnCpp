#include "Player.h"

Player::Player() 
    : playerSprite(idleTexture)
{
    idleTexture.loadFromFile("../assets/Soldier/Soldier-Idle.png");
    walkTexture.loadFromFile("../assets/Soldier/Soldier-Walk.png");
    playerSprite.setTexture(idleTexture);
    playerSprite.setTextureRect({{0,0}, {100, 100}});
    playerSprite.setOrigin({playerSprite.getTextureRect().size.x / 2.0f, playerSprite.getTextureRect().size.y / 2.0f });
    playerSprite.setScale({5.f, 5.f});
    playerSprite.setPosition({400,300});
}

void Player::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
        playerSprite.setTexture(walkTexture);
        playerSprite.setScale({5.f, 5.f});
        playerSprite.move({15.f, 0.f});
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
        playerSprite.setTexture(walkTexture);
        playerSprite.setScale({-5.f, 5.f});
        playerSprite.move({-15.f, 0.f});
    }
    else {
        playerSprite.setTexture(idleTexture);
    }
}

void Player::update(float deltaTime) {
    elapsedTimer += deltaTime;
    if (elapsedTimer >= frameDelay) {
        texWidth = (texWidth + 100) % playerSprite.getTexture().getSize().x;
        playerSprite.setTextureRect({{texWidth, 0}, {100, 100}});
        elapsedTimer = 0.f;
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(playerSprite);
}