#include "Player.h"
#include <iostream>

Player::Player() 
    : playerSprite(idleTexture)
{
    idleTexture.loadFromFile("../assets/Soldier/Soldier-Idle.png");
    walkTexture.loadFromFile("../assets/Soldier/Soldier-Walk.png");
    playerSprite.setTextureRect({{0,0}, {100, 100}});
    playerSprite.setOrigin({playerSprite.getTextureRect().size.x / 2.0f, playerSprite.getTextureRect().size.y / 2.0f });
    playerSprite.setScale({5.f, 5.f});
    playerSprite.setPosition({400,300});
}

void Player::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
        playerSprite.setTexture(walkTexture);
        playerSprite.setScale({5.f, 5.f});
        playerSprite.move({4.f, 0.f});
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
        playerSprite.setTexture(walkTexture);
        playerSprite.setScale({-5.f, 5.f});
        playerSprite.move({-4.f, 0.f});
    }
    else {
        playerSprite.setTexture(idleTexture);
    }
}

void Player::update(float deltaTime)
{
    elapsedTimer += deltaTime;
    auto bounds = playerSprite.getGlobalBounds();

    if (elapsedTimer >= frameDelay) {
        texWidth = (texWidth + 100) % playerSprite.getTexture().getSize().x;
        playerSprite.setTextureRect({{texWidth, 0}, {100, 100}});
        elapsedTimer = 0.f;
    }
    playerSprite.move({0.f, verticalVelocity});
    verticalVelocity += gravity;

    //std::cout << "y postition: "<< playerSprite.getPosition().y << "\n"<< "Global y size: " << bounds.size.y << std::endl;

    //Top Window Collision
    if (playerSprite.getPosition().y - (bounds.size.y)/10 < 0) {
        // Collided with or passed through the bottom
        playerSprite.setPosition(
            {playerSprite.getPosition().x,
            0 + bounds.size.y/10}
        );
        verticalVelocity = 0.f;  // stop downward movement
    }

    //Bottom Window Collision
    if (playerSprite.getPosition().y + (bounds.size.y)/10 > HEIGHT) {
        // Collided with or passed through the bottom
        playerSprite.setPosition(
            {playerSprite.getPosition().x,
            HEIGHT - bounds.size.y/10}
        );
        verticalVelocity = 0.f;  // stop downward movement
    }

    //Right Window Collision
    if(playerSprite.getPosition().x + (bounds.size.x)/10 > WIDTH) {
        playerSprite.setPosition(
            {WIDTH - bounds.size.x/10,
            playerSprite.getPosition().y}
        );
    }

    //std::cout << "x postition: "<< playerSprite.getPosition().x << "\n"<< "Global x size: " << bounds.size.x << std::endl;

    //Left Window Collision
    if(playerSprite.getPosition().x - (bounds.size.x)/10 < 0) {
        playerSprite.setPosition(
            {0 + bounds.size.x/10,
            playerSprite.getPosition().y}
        );
    }      

}

void Player::draw(sf::RenderWindow& window) {
    window.draw(playerSprite);
}