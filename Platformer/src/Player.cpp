#include "Player.h"
#include <iostream>

Player::Player() 
    : playerSprite(idleTexture), speed(4.f)
{
    idleTexture.loadFromFile("../assets/Soldier/Soldier-Idle.png");
    walkTexture.loadFromFile("../assets/Soldier/Soldier-Walk.png");
    playerSprite.setTextureRect({{0,0}, {100, 100}});
    playerSprite.setOrigin({playerSprite.getTextureRect().size.x / 2.0f, playerSprite.getTextureRect().size.y / 2.0f });
    playerSprite.setScale({2.f, 2.f});
    playerSprite.setPosition({300,250});
    setHitbox({{41.f, 41.f}, { 18.f, 18.f}}); 
}

void Player::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
        playerSprite.setTexture(walkTexture);
        playerSprite.setScale({2.f, 2.f});
        isMovingLeft = false;
        isMovingRight = true;
        playerSprite.move({speed, 0.f});
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space) && isGrounded) {
        //playerSprite.setTexture(walkTexture);
        playerSprite.move({0.f,-10.f});
        isGrounded = false;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
        playerSprite.setTexture(walkTexture);
        playerSprite.setScale({-2.f, 2.f});
        isMovingLeft = true;
        isMovingRight = false;
        playerSprite.move({-speed, 0.f});
    }
    else {
        playerSprite.setTexture(idleTexture);
        isMovingLeft = false;
        isMovingRight = false;
    }
}

void Player::setHitbox(const sf::FloatRect& hb){
    localHitbox = hb; 
}

sf::FloatRect Player::getGlobalHitbox() const{
    return playerSprite.getTransform().transformRect(localHitbox);
}


void Player::handleCollision(float deltaX, float deltaY)
{
    auto collisionRects = map.getCollisionRect();
    auto playerBox = getGlobalHitbox();
    for (const auto& rect : collisionRects) {
        if (auto opt = playerBox.findIntersection(rect)) {
            auto overlap = *opt;
            if (overlap.size.x < overlap.size.y) {
                if (deltaX > 0) {
                    playerSprite.move({-overlap.size.x, 0});
                } else {
                    playerSprite.move({overlap.size.x, 0});
                }
                break;
            }
        }
    }

    // Vertical phase:

    playerBox = getGlobalHitbox();
    for (const auto& rect : collisionRects) {
        if (auto opt = playerBox.findIntersection(rect)) {
            auto overlap = *opt;
            if (overlap.size.y <= overlap.size.x) {
                if (deltaY > 0) {
                    isGrounded = true;
                    playerSprite.move({0, -overlap.size.y});
                    verticalVelocity = 0;
                } else {
                    playerSprite.move({0, overlap.size.y});
                    verticalVelocity = 0;
                }
                break;
            }
        }
    }
}

void Player::update(float deltaTime)
{
    elapsedTimer += deltaTime;

    if (elapsedTimer >= frameDelay) {
        texWidth = (texWidth + 100) % playerSprite.getTexture().getSize().x;
        playerSprite.setTextureRect({{texWidth, 0}, {100, 100}});
        elapsedTimer = 0.f;
    }
    playerSprite.move({0.f, verticalVelocity});
    verticalVelocity += gravity;
    
    float moveX = (isMovingRight ? speed : isMovingLeft ? -speed : 0.f);

    handleCollision(moveX ,verticalVelocity);
    //std::cout << "PlayerX U: " << playerSprite.getPosition().x << "PlayerY U: " << playerSprite.getPosition().y << std::endl;

}

void Player::draw(sf::RenderWindow& window) {
    window.draw(playerSprite);
}