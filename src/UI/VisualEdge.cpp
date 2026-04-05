#include "VisualEdge.h"

#include <cmath>

namespace UI {

VisualEdge::VisualEdge(const sf::Vector2f& start,
                       const sf::Vector2f& end,
                       float thickness,
                       const sf::Color& color)
    : startPoint(start), endPoint(end), edgeThickness(thickness), edgeColor(color) {}

void VisualEdge::setPoints(const sf::Vector2f& start, const sf::Vector2f& end) {
    startPoint = start;
    endPoint = end;
}

void VisualEdge::setThickness(float thickness) {
    edgeThickness = thickness;
}

void VisualEdge::setColor(const sf::Color& color) {
    edgeColor = color;
}

sf::Vector2f VisualEdge::midpoint() const {
    return {(startPoint.x + endPoint.x) * 0.5f, (startPoint.y + endPoint.y) * 0.5f};
}

sf::Vector2f VisualEdge::normal() const {
    const sf::Vector2f delta(endPoint.x - startPoint.x, endPoint.y - startPoint.y);
    const float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    if (length <= 0.001f) {
        return {0.0f, 0.0f};
    }
    return {-delta.y / length, delta.x / length};
}

sf::Vector2f VisualEdge::labelPosition(float offset, float side) const {
    const sf::Vector2f mid = midpoint();
    const sf::Vector2f n = normal();
    if (n.x == 0.0f && n.y == 0.0f) {
        return mid;
    }
    return {mid.x + n.x * offset * side, mid.y + n.y * offset * side};
}

void VisualEdge::render(sf::RenderWindow& window) const {
    const sf::Vector2f delta(endPoint.x - startPoint.x, endPoint.y - startPoint.y);
    const float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    if (length <= 0.001f) {
        return;
    }

    const sf::Vector2f n(-delta.y / length, delta.x / length);
    const sf::Vector2f offset(n.x * edgeThickness * 0.5f, n.y * edgeThickness * 0.5f);

    sf::Vertex thickEdge[] = {
        sf::Vertex(sf::Vector2f(startPoint.x + offset.x, startPoint.y + offset.y), edgeColor),
        sf::Vertex(sf::Vector2f(startPoint.x - offset.x, startPoint.y - offset.y), edgeColor),
        sf::Vertex(sf::Vector2f(endPoint.x + offset.x, endPoint.y + offset.y), edgeColor),
        sf::Vertex(sf::Vector2f(endPoint.x - offset.x, endPoint.y - offset.y), edgeColor)
    };
    window.draw(thickEdge, 4, sf::PrimitiveType::TriangleStrip);
}

} // namespace UI
