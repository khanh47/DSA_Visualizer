#include "BaseVisualizer.h"
#include <algorithm>

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
            isDragging = true;
            lastMousePos = sf::Vector2f(static_cast<float>(mouseButton->position.x), 
                                        static_cast<float>(mouseButton->position.y));
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
                sf::Vector2f delta = currentMousePos - lastMousePos;
                panOffset += delta;
                lastMousePos = currentMousePos;
            }
        }
    }
}
