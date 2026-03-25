#include "ButtonMenu.h"

namespace UI {

void ButtonMenu::setLayoutProperties(const sf::Vector2f& startPosition,
                                     const sf::Vector2f& buttonSize,
                                     float spacing,
                                     bool horizontal,
                                     const sf::Color& defaultColor,
                                     unsigned int defaultCharSize) {
    _layout.startPosition = startPosition;
    _layout.buttonSize = buttonSize;
    _layout.spacing = spacing;
    _layout.horizontal = horizontal;
    _layout.defaultColor = defaultColor;
    _layout.defaultCharSize = defaultCharSize;

    for (std::size_t i = 0; i < _buttonMenu.size(); ++i) {
        const float offset = static_cast<float>(i) * _layout.spacing;
        const sf::Vector2f position = _layout.horizontal
            ? sf::Vector2f(_layout.startPosition.x + offset, _layout.startPosition.y)
            : sf::Vector2f(_layout.startPosition.x, _layout.startPosition.y + offset);

        _buttonMenu[i]->setSize(_layout.buttonSize);
        _buttonMenu[i]->setPosition(position);
    }
}

void ButtonMenu::addButton(const std::shared_ptr<Button>& button) {
    if (!button) {
        return;
    }

    _buttonMenu.push_back(button);
}

void ButtonMenu::addButtonAuto(const std::string& text, std::unique_ptr<ICommand> command) {
    addButtonAuto(text, _layout.defaultCharSize, std::move(command), _layout.defaultColor);
}

void ButtonMenu::addButtonAuto(const std::string& text, unsigned int charSize, 
                                std::unique_ptr<ICommand> command, 
                                const sf::Color& color) {
    const float offset = static_cast<float>(_buttonMenu.size()) * _layout.spacing;
    const sf::Vector2f position = _layout.horizontal
        ? sf::Vector2f(_layout.startPosition.x + offset, _layout.startPosition.y)
        : sf::Vector2f(_layout.startPosition.x, _layout.startPosition.y + offset);

    auto button = std::make_shared<Button>(
        position,
        _layout.buttonSize,
        color,
        text,
        charSize
    );
    button->setCommand(std::move(command));
    addButton(button);
}

void ButtonMenu::processEvent(const sf::Event& event) {
    for (const std::shared_ptr<Button>& button : _buttonMenu) {
        button->processEvent(event);
    }
}

void ButtonMenu::render(sf::RenderWindow& window) {
    for (const std::shared_ptr<Button>& button : _buttonMenu) {
        button->render(window);
    }
}

void ButtonMenu::clear() {
    _buttonMenu.clear();
}

std::size_t ButtonMenu::size() const {
    return _buttonMenu.size();
}

} // namespace UI
