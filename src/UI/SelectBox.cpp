#include "SelectBox.h"
#include "ResourceManager.h"

namespace UI {

SelectBox::SelectBox(const sf::Vector2f& position,
                     const sf::Vector2f& size,
                     const std::vector<std::string>& options,
                     unsigned int charSize)
    : mainText(ResourceManager::getInstance().getFont("Roboto"), "", charSize),
      arrowText(ResourceManager::getInstance().getFont("Roboto"), "v", charSize),
      options(options),
      expanded(false),
      selectedIndex(0) {
    mainBox.setPosition(position);
    mainBox.setSize(size);
    mainBox.setFillColor(sf::Color(58, 66, 88));

    mainText.setFillColor(sf::Color::White);
    mainText.setPosition({position.x + 12.0f, position.y + 8.0f});

    arrowText.setFillColor(sf::Color(170, 178, 196));
    arrowText.setPosition({position.x + size.x - 24.0f, position.y + 7.0f});

    updateMainText();

    const float rowHeight = size.y;
    optionBoxes.reserve(options.size());
    optionTexts.reserve(options.size());

    for (std::size_t i = 0; i < options.size(); ++i) {
        sf::RectangleShape optionBox;
        optionBox.setPosition({position.x, position.y + rowHeight * static_cast<float>(i + 1)});
        optionBox.setSize(size);
        optionBox.setFillColor(sf::Color(44, 52, 74));
        optionBox.setOutlineThickness(1.0f);
        optionBox.setOutlineColor(sf::Color(100, 110, 140));
        optionBoxes.push_back(optionBox);

        sf::Text optionText(ResourceManager::getInstance().getFont("Roboto"), options[i], charSize);
        optionText.setFillColor(sf::Color::White);
        optionText.setPosition({position.x + 12.0f, position.y + rowHeight * static_cast<float>(i + 1) + 8.0f});
        optionTexts.push_back(optionText);
    }
}

void SelectBox::processEvent(const sf::Event& event) {
    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button != sf::Mouse::Button::Left) {
            return;
        }

        const sf::Vector2f mousePos(static_cast<float>(mouseEvent->position.x),
                                    static_cast<float>(mouseEvent->position.y));

        if (mainBox.getGlobalBounds().contains(mousePos)) {
            expanded = !expanded;
            return;
        }

        if (expanded) {
            for (std::size_t i = 0; i < optionBoxes.size(); ++i) {
                if (optionBoxes[i].getGlobalBounds().contains(mousePos)) {
                    selectedIndex = i;
                    updateMainText();
                    expanded = false;
                    return;
                }
            }
        }

        expanded = false;
    }
}

void SelectBox::render(sf::RenderWindow& window) {
    window.draw(mainBox);
    window.draw(mainText);
    window.draw(arrowText);

    if (!expanded) {
        return;
    }

    for (std::size_t i = 0; i < optionBoxes.size(); ++i) {
        if (i == selectedIndex) {
            optionBoxes[i].setFillColor(sf::Color(120, 90, 235));
        } else {
            optionBoxes[i].setFillColor(sf::Color(44, 52, 74));
        }

        window.draw(optionBoxes[i]);
        window.draw(optionTexts[i]);
    }
}

std::string SelectBox::getSelected() const {
    if (options.empty()) {
        return "";
    }

    return options[selectedIndex];
}

void SelectBox::setSelected(std::size_t index) {
    if (index >= options.size()) {
        return;
    }

    selectedIndex = index;
    updateMainText();
}

void SelectBox::updateMainText() {
    if (options.empty()) {
        mainText.setString("No options");
        return;
    }

    mainText.setString(options[selectedIndex]);
}

} // namespace UI
