#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace UI {

class TextBox {
public:
    TextBox(const sf::Vector2f& position,
            const sf::Vector2f& size,
            const std::string& description,
            unsigned int charSize = 22,
            std::size_t maxChars = 32);

    void processEvent(const sf::Event& event);
    void render(sf::RenderWindow& window);

    const std::string& getText() const;
    void setText(const std::string& value);
    void setDescription(const std::string& description);

private:
    void refreshDisplay();

    sf::RectangleShape box;
    sf::Text textDrawable;
    std::string text;
    std::string description;
    bool focused;
    std::size_t maxChars;
};

} // namespace UI
