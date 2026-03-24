#pragma once

#include <SFML/Graphics.hpp>

class VisualizationScene;

class BasePanel {
protected:
    sf::Vector2f position;
    sf::Vector2f size;

public:
    BasePanel(VisualizationScene& scene, const sf::Vector2f& position, const sf::Vector2f& size)
        : position(position), size(size) {
    }

    virtual ~BasePanel() = default;
};
