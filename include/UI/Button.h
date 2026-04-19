#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <functional>
#include "ICommand.h"
#include <optional>

namespace UI {

class Button {
public:
    Button(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& color, 
           const std::string& text, unsigned int charSize, 
           float cornerRadius = 10.0f, const std::string& iconAlias = "");

    void setCommand(std::unique_ptr<ICommand> command);
    void setPosition(const sf::Vector2f& position);
    void setSize(const sf::Vector2f& size);
    void processEvent(const sf::Event& event);
    void render(sf::RenderWindow& window);

private:

    sf::ConvexShape shape; // Using ConvexShape instead of RectangleShape for rounded corners
    sf::Text label;
    std::optional<sf::Sprite> icon;        // Sprite to hold the button's image
    bool hasIcon = false;  // Flag to check if an image is provided
    float cornerRadius;    // Radius for the rounded corners

    std::unique_ptr<ICommand> buttonCommand;

    sf::Color defaultColor;
    sf::Color hoverColor;
    bool isHovered = false;

    // --- ADD THESE VARIABLES FOR THE LIFT EFFECT ---
    sf::Vector2f basePosition; // Stores the original, un-lifted position
    sf::Vector2f baseSize;     // Stores the original size
    const float liftAmount = 3.0f; // How many pixels the button moves up when hovered

    // Helper function to calculate and set the points for the rounded rectangle
    void updateRoundedShape(const sf::Vector2f& position, const sf::Vector2f& size);
    
    // Helper function to align text and icon perfectly in the center
    void updateLayout(const sf::Vector2f& position, const sf::Vector2f& size);

        // Constructor now accepts cornerRadius (default 10) and iconAlias (default empty)

};
}
