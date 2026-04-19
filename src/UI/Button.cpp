
#include "Button.h"
#include "ResourceManager.h"
#include <algorithm>
#include <cmath> 

namespace UI {

Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& color, 
               const std::string& text, unsigned int charSize, 
               float radius, const std::string& iconAlias)
    : label(ResourceManager::getInstance().getFont("Roboto"), text, charSize),
      defaultColor(color), cornerRadius(radius),basePosition(position), baseSize(size) {
      
    int r = std::min(255, color.r + 40);
    int g = std::min(255, color.g + 40);
    int b = std::min(255, color.b + 40);
    hoverColor = sf::Color(r, g, b, color.a);

    label.setFillColor(sf::Color::White);

    if (!iconAlias.empty()) {
        try {
            sf::Texture& tex = ResourceManager::getInstance().getTexture(iconAlias);
            icon.emplace(tex); // Tạo sprite và nhét texture vào cùng lúc
            
            float iconTargetHeight = size.y * 0.6f;
            float scale = iconTargetHeight / tex.getSize().y;
            icon->setScale({scale, scale});
        } catch (const std::exception& e) {
            icon.reset(); // Không tìm thấy ảnh thì hủy bỏ
        }
    }

    shape.setFillColor(defaultColor);
    updateRoundedShape(position, size);
    updateLayout(position, size);
}

void Button::updateRoundedShape(const sf::Vector2f& position, const sf::Vector2f& size) {
    const int pointsPerCorner = 10;
    shape.setPointCount(pointsPerCorner * 4);

    float actualRadius = std::min({cornerRadius, size.x / 2.0f, size.y / 2.0f});
    const float pi = 3.141592654f;
    int pointIndex = 0;

    auto addCorner = [&](float cx, float cy, float startAngle) {
        for (int i = 0; i < pointsPerCorner; ++i) {
            float angle = startAngle + (i * (pi / 2.0f) / (pointsPerCorner - 1));
            float px = cx + actualRadius * std::cos(angle);
            float py = cy + actualRadius * std::sin(angle);
            shape.setPoint(pointIndex++, sf::Vector2f(px, py));
        }
    };

    addCorner(size.x - actualRadius, size.y - actualRadius, 0.0f);
    addCorner(actualRadius, size.y - actualRadius, pi / 2.0f);
    addCorner(actualRadius, actualRadius, pi);
    addCorner(size.x - actualRadius, actualRadius, 3.0f * pi / 2.0f);

    shape.setPosition(position);
}

void Button::updateLayout(const sf::Vector2f& position, const sf::Vector2f& size) {
    float centerX = position.x + size.x / 2.0f;

    // 1. Setup Label Origin
    sf::FloatRect textBounds = label.getLocalBounds();
    label.setOrigin({textBounds.position.x + textBounds.size.x / 2.0f,
                     textBounds.position.y + textBounds.size.y / 2.0f});

    // 2. Position Label at a FIXED vertical coordinate
    // We place the text at 82% of the button's height. 
    // This ensures all button texts are perfectly aligned horizontally.
    float fixedLabelY = position.y + (size.y * 0.82f);
    label.setPosition({centerX, fixedLabelY});

    if (icon.has_value()) {
        sf::FloatRect iconBounds = icon->getLocalBounds();
        
        // 3. Scale icon to fit the reserved upper area
        // We allow the icon to occupy 75% of width and 55% of height
        float maxIconW = size.x * 0.75f; 
        float maxIconH = size.y * 0.55f;

        float scale = std::min(maxIconW / iconBounds.size.x, maxIconH / iconBounds.size.y);
        icon->setScale({scale, scale});

        // 4. Position Icon centered in the UPPER area of the button
        // We place the icon center at 40% of the button's height
        float fixedIconCenterY = position.y + (size.y * 0.40f);
        
        icon->setOrigin({iconBounds.position.x + iconBounds.size.x / 2.0f,
                         iconBounds.position.y + iconBounds.size.y / 2.0f});
        icon->setPosition({centerX, fixedIconCenterY});

    } else {
        // If there's no icon (e.g. Back button), center the text perfectly in the button
        label.setPosition({centerX, position.y + size.y / 2.0f});
    }
}

void Button::setPosition(const sf::Vector2f& position) {
    basePosition = position;
    shape.setPosition(position);
    sf::FloatRect bounds = shape.getLocalBounds();
    sf::Vector2f size(bounds.size.x, bounds.size.y);
    baseSize = size; 
    updateLayout(position, size);
}

void Button::setSize(const sf::Vector2f& size) {
    baseSize = size;
    updateRoundedShape(shape.getPosition(), size);
    updateLayout(shape.getPosition(), size);
}

void Button::setCommand(std::unique_ptr<ICommand> command) {
    buttonCommand = std::move(command);
}

void Button::processEvent(const sf::Event& event) {
    // 1. HANDLE HOVER (POP-UP EFFECT)
    if (const auto* mouseMove = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2f mousePos(static_cast<float>(mouseMove->position.x), static_cast<float>(mouseMove->position.y));
        bool wasHovered = isHovered;
        
        // IMPORTANT: Check collision against the BASE position, NOT the current moving shape.
        // This prevents the button from jittering when the mouse is at the bottom edge.
        sf::FloatRect staticBounds{basePosition, baseSize};
        isHovered = staticBounds.contains(mousePos); 

        if (isHovered) {
            shape.setFillColor(hoverColor);
        } else {
            shape.setFillColor(defaultColor);
        }
    }

    // 2. HANDLE CLICK EVENT
    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos(static_cast<float>(mouseEvent->position.x), static_cast<float>(mouseEvent->position.y));
            
            // Check click against the static bounds as well to be consistent
            sf::FloatRect staticBounds{basePosition, baseSize};
            if (staticBounds.contains(mousePos)) {
                if (buttonCommand) buttonCommand->execute();
            }
        }
    }
}

void Button::render(sf::RenderWindow& window) {
    // 1. Calculate the current draw position based on hover state
    sf::Vector2f drawPos = basePosition;
    if (isHovered) {
        drawPos.y -= liftAmount; // Visually lift the button up
    }

    // 2. Temporarily move the shape and elements to the draw position
    shape.setPosition(drawPos);
    
    // We need to re-center the label and icon relative to the new drawPos
    updateLayout(drawPos, baseSize);

    // 3. Draw everything
    window.draw(shape);
    if (icon.has_value()) {
        window.draw(icon.value());
    }
    if (!label.getString().isEmpty()) {
        window.draw(label);
    }

    // 4. Optional: Reset shape position back to base to keep physics/collision consistent
    shape.setPosition(basePosition);
}

} // namespace UI