#pragma once
#include <SFML/Graphics.hpp>
#include "SceneManager.h"

class App {
private:
    std::unique_ptr<SceneManager> manager;
    sf::RenderWindow window;
    sf::Clock dtClock; // Keep track of times between frames
    void render();
    void update(float deltaTime);
    void processEvents();

public:
    App();
    void run();
};