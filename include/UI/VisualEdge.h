#pragma once

#include <SFML/Graphics.hpp>

namespace UI {

class VisualEdge {
public:
    VisualEdge(const sf::Vector2f& start,
               const sf::Vector2f& end,
               float thickness = 4.0f,
               const sf::Color& color = sf::Color::Black);

    void setPoints(const sf::Vector2f& start, const sf::Vector2f& end);
    void setThickness(float thickness);
    void setColor(const sf::Color& color);

    sf::Vector2f midpoint() const;
    sf::Vector2f normal() const;
    sf::Vector2f labelPosition(float offset, float side = 1.0f) const;

    void render(sf::RenderWindow& window) const;

private:
    sf::Vector2f startPoint;
    sf::Vector2f endPoint;
    float edgeThickness = 4.0f;
    sf::Color edgeColor = sf::Color(120, 130, 150);
};

} // namespace UI
