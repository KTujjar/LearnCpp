#include "Game.h"
#include <iostream>
#include <cmath>

// Game constructor - use default-initialized sprite
Game::Game()
    : window(sf::VideoMode({Global::windowWidth, Global::windowHeight}), "Platformer", sf::State::Fullscreen)
{
    auto size = window.getSize();
    Global::windowWidth = size.x;
    Global::windowHeight = size.y;
    window.setFramerateLimit(60);
    sf::View view = window.getDefaultView();
    view.zoom(0.33f);
    view.setCenter(view.getSize() * 0.5f);
    window.setView(view)
;}

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