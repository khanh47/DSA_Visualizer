#pragma once
#include <SFML/Graphics.hpp>

class BaseVisualizer {
public:
    virtual ~BaseVisualizer() = default;
    virtual void handleEvent(const sf::Event& event, const sf::RenderWindow& window) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void reset() {} 
};;
