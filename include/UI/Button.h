#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <functional>
#include "ICommand.h"

namespace UI {

class Button {
public:
    Button(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& color, const std::string& text, unsigned int charSize);

    void setCommand(std::unique_ptr<ICommand> command);
    void setPosition(const sf::Vector2f& position);
    void setSize(const sf::Vector2f& size);
    void processEvent(const sf::Event& event);
    void render(sf::RenderWindow& window);

private:
    sf::RectangleShape shape;
    sf::Text label;
    std::unique_ptr<ICommand> buttonCommand;
};
}
