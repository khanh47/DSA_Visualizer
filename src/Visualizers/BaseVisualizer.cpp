#include "BaseVisualizer.h"
#include <algorithm>

void BaseVisualizer::setDragRegion(float topY, float bottomY) {
    dragRegionTopY = topY;
    dragRegionBottomY = std::max(topY, bottomY);
}

bool BaseVisualizer::isInsideDragRegion(const sf::Vector2f& position) const {
    return position.y >= dragRegionTopY && position.y <= dragRegionBottomY;
}

void BaseVisualizer::handleZoomPanEvents(const sf::Event& event) {
    if (event.is<sf::Event::MouseWheelScrolled>()) {
        // Zoom in/out with mouse wheel
        auto mouseScroll = event.getIf<sf::Event::MouseWheelScrolled>();
        if (mouseScroll) {
            float zoomFactor = 1.1f;
            if (mouseScroll->delta > 0) {
                zoomLevel *= zoomFactor;
            } else if (zoomLevel > 0.5f) {
                zoomLevel /= zoomFactor;
            }
            zoomLevel = std::max(0.5f, std::min(3.0f, zoomLevel));
        }
    } else if (event.is<sf::Event::MouseButtonPressed>()) {
        auto mouseButton = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouseButton && mouseButton->button == sf::Mouse::Button::Left) {
            const sf::Vector2f pressedPos(static_cast<float>(mouseButton->position.x),
                                          static_cast<float>(mouseButton->position.y));
            isDragging = isInsideDragRegion(pressedPos);
            lastMousePos = pressedPos;
        }
    } else if (event.is<sf::Event::MouseButtonReleased>()) {
        auto mouseButton = event.getIf<sf::Event::MouseButtonReleased>();
        if (mouseButton && mouseButton->button == sf::Mouse::Button::Left) {
            isDragging = false;
        }
    } else if (event.is<sf::Event::MouseMoved>()) {
        if (isDragging) {
            auto mouseMove = event.getIf<sf::Event::MouseMoved>();
            if (mouseMove) {
                sf::Vector2f currentMousePos(static_cast<float>(mouseMove->position.x), 
                                            static_cast<float>(mouseMove->position.y));
                if (!isInsideDragRegion(currentMousePos)) {
                    isDragging = false;
                    return;
                }
                sf::Vector2f delta = currentMousePos - lastMousePos;
                panOffset += delta;
                lastMousePos = currentMousePos;
            }
        }
    }
}
