#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace UI {

class VisualNode {
public:
    VisualNode(sf::Font& font, const std::string& value, float radius = 28.0f)
        : label(font, value, 20) {
        circle.setRadius(radius);
        circle.setOrigin({radius, radius});
        circle.setFillColor(sf::Color(57, 74, 111));
        circle.setOutlineColor(sf::Color(200, 220, 255));
        circle.setOutlineThickness(2.0f);

        label.setFillColor(sf::Color::White);
        updateLabelOrigin();
    }

    void setPosition(const sf::Vector2f& position) {
        circle.setPosition(position);
        label.setPosition(position);
    }

    void setValue(const std::string& value) {
        label.setString(value);
        updateLabelOrigin();
    }

    void setFillColor(const sf::Color& color) {
        circle.setFillColor(color);
    }

    void setOutlineColor(const sf::Color& color) {
        circle.setOutlineColor(color);
    }

    void render(sf::RenderWindow& window) const {
        window.draw(circle);
        window.draw(label);
    }

private:
    void updateLabelOrigin() {
        const sf::FloatRect bounds = label.getLocalBounds();
        label.setOrigin({bounds.position.x + bounds.size.x * 0.5f,
                         bounds.position.y + bounds.size.y * 0.5f});
    }

    sf::CircleShape circle;
    sf::Text label;
};

} // namespace UI
