#pragma once
#include <cstddef>
#include <memory>
#include <vector>
#include <string>
#include "Button.h"
#include "ICommand.h"

namespace UI {
class ButtonMenu {
private:
    struct LayoutProperties {
        sf::Vector2f startPosition = {20.0f, 80.0f};
        sf::Vector2f buttonSize = {220.0f, 60.0f};
        float spacing = 70.0f;
        bool horizontal = false;
        sf::Color defaultColor = sf::Color(100, 149, 237);
        unsigned int defaultCharSize = 28;
    };

    std::vector<std::shared_ptr<Button>> _buttonMenu;
    LayoutProperties _layout;

public:
    ButtonMenu() = default;

    void setLayoutProperties(const sf::Vector2f& startPosition,
                             const sf::Vector2f& buttonSize,
                             float spacing,
                             bool horizontal,
                             const sf::Color& defaultColor,
                             unsigned int defaultCharSize);

    void addButton(const std::shared_ptr<Button>& button);
    void addButtonAuto(const std::string& text, std::unique_ptr<ICommand> command);
    void addButtonAuto(const std::string& text, unsigned int charSize, 
                       std::unique_ptr<ICommand> command, 
                       const sf::Color& color = sf::Color(100, 149, 237));
    void processEvent(const sf::Event& event);
    void render(sf::RenderWindow& window);

    void clear();
    std::size_t size() const;
};
} // namespace UI