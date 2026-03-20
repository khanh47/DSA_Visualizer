#include "Button.h"
#include "ResourceManager.h"

namespace UI {

Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& color, const std::string& text, unsigned int charSize)
    : label(ResourceManager::getInstance().getFont("Roboto"), text, charSize) {
    // Setup shape
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(color); 

    // Setup label
    label.setFillColor(sf::Color::White);

    // Center text
    const sf::FloatRect textBounds = label.getLocalBounds();
    label.setOrigin({textBounds.position.x + textBounds.size.x / 2.0f,
                     textBounds.position.y + textBounds.size.y / 2.0f});
    label.setPosition({position.x + size.x / 2.0f, position.y + size.y / 2.0f});
}

void Button::setCommand(std::unique_ptr<ICommand> command) {
    buttonCommand = std::move(command);
}

void Button::processEvent(const sf::Event& event) {
    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            const sf::Vector2f mousePos(static_cast<float>(mouseEvent->position.x),
                                        static_cast<float>(mouseEvent->position.y));
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
