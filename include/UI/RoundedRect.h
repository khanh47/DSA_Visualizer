#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

namespace UI {

/// Creates a ConvexShape representing a rounded rectangle.
/// Can be used as a drop-in replacement for RectangleShape when rounded corners are needed.
inline sf::ConvexShape makeRoundedRect(const sf::Vector2f& size,
                                        float cornerRadius,
                                        int pointsPerCorner = 8) {
    sf::ConvexShape shape;
    shape.setPointCount(pointsPerCorner * 4);

    float r = std::min({cornerRadius, size.x / 2.0f, size.y / 2.0f});
    const float pi = 3.141592654f;
    int idx = 0;

    auto addCorner = [&](float cx, float cy, float startAngle) {
        for (int i = 0; i < pointsPerCorner; ++i) {
            float angle = startAngle + (i * (pi / 2.0f) / (pointsPerCorner - 1));
            float px = cx + r * std::cos(angle);
            float py = cy + r * std::sin(angle);
            shape.setPoint(idx++, {px, py});
        }
    };

    // Bottom-right, bottom-left, top-left, top-right
    addCorner(size.x - r, size.y - r, 0.0f);
    addCorner(r, size.y - r, pi / 2.0f);
    addCorner(r, r, pi);
    addCorner(size.x - r, r, 3.0f * pi / 2.0f);

    return shape;
}

} // namespace UI
