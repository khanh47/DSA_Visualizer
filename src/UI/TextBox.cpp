#include "TextBox.h"
#include "RoundedRect.h"
#include "ResourceManager.h"

namespace UI {

TextBox::TextBox(const sf::Vector2f& position,
                 const sf::Vector2f& size,
                 const std::string& description,
                 unsigned int charSize,
                 std::size_t maxChars)
    : textDrawable(ResourceManager::getInstance().getFont("Roboto"), "", charSize),
      description(description),
      focused(false),
      maxChars(maxChars),
      boxPosition(position),
      boxSize(size) {

    box = makeRoundedRect(size, kCornerRadius);
    box.setPosition(position);
    box.setFillColor(sf::Color::White);
    box.setOutlineThickness(2.0f);
    box.setOutlineColor(sf::Color(120, 150, 230));

    refreshDisplay();
}

void TextBox::processEvent(const sf::Event& event) {
    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            const sf::Vector2f mousePos(static_cast<float>(mouseEvent->position.x),
                                        static_cast<float>(mouseEvent->position.y));
            // Hit-test using stored position/size
            sf::FloatRect bounds(boxPosition, boxSize);
            focused = bounds.contains(mousePos);
            box.setOutlineColor(focused ? sf::Color(152, 180, 255) : sf::Color(120, 150, 230));
        }
    }

    if (!focused) {
        return;
    }

    if (const auto* textEvent = event.getIf<sf::Event::TextEntered>()) {
        const std::uint32_t unicode = textEvent->unicode;

        if (unicode == 8) {
            if (!text.empty()) {
                text.pop_back();
            }
            refreshDisplay();
            return;
        }

        if (unicode >= 32 && unicode <= 126 && text.size() < maxChars) {
            text.push_back(static_cast<char>(unicode));
            refreshDisplay();
        }
    }
}

void TextBox::render(sf::RenderWindow& window) {
    window.draw(box);
    window.draw(textDrawable);
}

const std::string& TextBox::getText() const {
    return text;
}

void TextBox::setText(const std::string& value) {
    text = value;
    if (text.size() > maxChars) {
        text.resize(maxChars);
    }
    refreshDisplay();
}

void TextBox::setDescription(const std::string& value) {
    description = value;
    refreshDisplay();
}

void TextBox::refreshDisplay() {
    if (text.empty()) {
        textDrawable.setString(description);
        textDrawable.setFillColor(sf::Color(150, 150, 150));
    } else {
        textDrawable.setString(text);
        textDrawable.setFillColor(sf::Color(25, 25, 25));
    }

    const sf::FloatRect bounds = textDrawable.getLocalBounds();
    textDrawable.setOrigin({bounds.position.x + bounds.size.x / 2.0f,
                            bounds.position.y + bounds.size.y / 2.0f});

    textDrawable.setPosition({boxPosition.x + boxSize.x / 2.0f,
                              boxPosition.y + boxSize.y / 2.0f});
}

} // namespace UI
