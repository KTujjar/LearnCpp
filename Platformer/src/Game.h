#pragma once
#include <SFML/Graphics.hpp>
#include "Global.h"
#include "Player.h"
#include "Map.h"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();

    int width = 800;
    int height = 600;
    sf::RenderWindow window;

    sf::Clock animClock;

    Player player;
    Map map;

    sf::Clock deltaClock;     // For calculating deltaTime
    float deltaTime = 0.f;    // Stores time passed between frames
};