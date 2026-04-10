#include "VisualNode.h"

#include <algorithm>

namespace UI {

VisualNode::VisualNode(sf::Font& font, const std::string& value, float radius)
    : label(font, value, 20) {
    const unsigned int textSize = static_cast<unsigned int>(std::clamp(radius * 0.7f, 10.0f, 48.0f));
    label.setCharacterSize(textSize);

    circle.setRadius(radius);
    circle.setOrigin({radius, radius});
    circle.setFillColor(sf::Color::White);
    circle.setOutlineColor(sf::Color::Black);
    circle.setOutlineThickness(2.5f);

    label.setFillColor(sf::Color::Black);
    updateLabelOrigin();
}

void VisualNode::setPosition(const sf::Vector2f& position) {
    circle.setPosition(position);
    label.setPosition(position);
}

void VisualNode::setValue(const std::string& value) {
    label.setString(value);
    updateLabelOrigin();
}

void VisualNode::setFillColor(const sf::Color& color) {
    circle.setFillColor(color);
}

void VisualNode::setOutlineColor(const sf::Color& color) {
    circle.setOutlineColor(color);
}

void VisualNode::setAlpha(std::uint8_t alpha) {
    sf::Color fillColor = circle.getFillColor();
    fillColor.a = alpha;
    circle.setFillColor(fillColor);

    sf::Color outlineColor = circle.getOutlineColor();
    outlineColor.a = alpha;
    circle.setOutlineColor(outlineColor);

    sf::Color textColor = label.getFillColor();
    textColor.a = alpha;
    label.setFillColor(textColor);
}

void VisualNode::setRadius(float radius) {
    circle.setRadius(radius);
    circle.setOrigin({radius, radius});
    // Re-scale text to fit
    const unsigned int textSize = static_cast<unsigned int>(std::clamp(radius * 0.7f, 10.0f, 48.0f));
    label.setCharacterSize(textSize);
    updateLabelOrigin();
}

void VisualNode::render(sf::RenderWindow& window) const {
    window.draw(circle);
    window.draw(label);
}

void VisualNode::updateLabelOrigin() {
    const sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin({bounds.position.x + bounds.size.x * 0.5f,
                     bounds.position.y + bounds.size.y * 0.5f});
}

} // namespace UI
