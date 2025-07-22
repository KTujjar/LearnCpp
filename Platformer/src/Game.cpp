#include "Game.h"
#include <iostream>
#include <cmath>

// Game constructor - use default-initialized sprite
Game::Game()
    : window(sf::VideoMode({WIDTH, HEIGHT}), "SFML Game")
{
    window.setFramerateLimit(60);
}

void Game::run() {
    while (window.isOpen()) {
        deltaTime = deltaClock.restart().asSeconds();
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

void Game::update() {
    player.handleInput();
    player.update(deltaTime);
}

void Game::render() {
    window.clear();
    player.draw(window);
    map.draw(window);
    window.display();
}