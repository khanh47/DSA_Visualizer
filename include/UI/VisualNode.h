#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace UI {

class VisualNode {
public:
    VisualNode(sf::Font& font, const std::string& value, float radius = 35.0f);

    void setPosition(const sf::Vector2f& position);
    void setValue(const std::string& value);
    void setFillColor(const sf::Color& color);
    void setOutlineColor(const sf::Color& color);
    void render(sf::RenderWindow& window) const;

private:
    void updateLabelOrigin();

    sf::CircleShape circle;
    sf::Text label;
};

} // namespace UI
