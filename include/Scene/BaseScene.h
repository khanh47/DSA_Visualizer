#pragma once

#include <SFML/Graphics.hpp>

class BaseScene {
public:
    virtual ~BaseScene() = default;
    virtual void processEvents(const sf::Event& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};