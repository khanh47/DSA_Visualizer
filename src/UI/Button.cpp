#include "Button.h"
#include "ResourceManager.h"

namespace UI {

Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& color, const std::string& text, unsigned int charSize) {
    // Setup shape
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(color); // Default color

    // Setup label
    sf::Font &font = ResourceManager::getInstance().getFont("Roboto");
    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(charSize);
    label.setFillColor(sf::Color::White);

    // Center text
    sf::FloatRect textBounds = label.getLocalBounds();
    label.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    label.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);
}

void Button::setCommand(std::unique_ptr<ICommand> command) {
    buttonCommand = std::move(command);
}

void Button::processEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
            if (shape.getGlobalBounds().contains(mousePos)) {
                if (buttonCommand) {
                    buttonCommand->execute();
                }
            }
        }
    }
}

void Button::render(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(label);
}

}
