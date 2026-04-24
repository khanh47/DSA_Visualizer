#include "SelectBox.h"
#include "RoundedRect.h"
#include "ResourceManager.h"

namespace UI {

SelectBox::SelectBox(const sf::Vector2f& position,
                     const sf::Vector2f& size,
                     const std::vector<std::string>& options,
                     unsigned int charSize)
    : mainText(ResourceManager::getInstance().getFont("Roboto"), "", charSize),
      arrowText(ResourceManager::getInstance().getFont("Roboto"), "v", charSize),
      boxPosition(position),
      boxSize(size),
      options(options),
      expanded(false),
      selectedIndex(0) {

    // Main rounded box
    mainBox = makeRoundedRect(size, kCornerRadius);
    mainBox.setPosition(position);
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
        float optY = position.y + rowHeight * static_cast<float>(i + 1);

        // Only round bottom corners on the last option
        sf::ConvexShape optionBox = makeRoundedRect(size,
            (i == options.size() - 1) ? kCornerRadius : 0.0f);
        optionBox.setPosition({position.x, optY});
        optionBox.setFillColor(sf::Color(44, 52, 74));
        optionBox.setOutlineThickness(1.0f);
        optionBox.setOutlineColor(sf::Color(100, 110, 140));
        optionBoxes.push_back(optionBox);

        sf::Text optionText(ResourceManager::getInstance().getFont("Roboto"), options[i], charSize);
        optionText.setFillColor(sf::Color::White);
        optionText.setPosition({position.x + 12.0f, optY + 8.0f});
        optionTexts.push_back(optionText);
    }
}

void SelectBox::processEvent(const sf::Event& event) {
    // ── Hover tracking ──────────────────────────────────────────────────
    if (const auto* mouseMove = event.getIf<sf::Event::MouseMoved>()) {
        if (expanded) {
            const sf::Vector2f mousePos(static_cast<float>(mouseMove->position.x),
                                        static_cast<float>(mouseMove->position.y));
            hoveredIndex = -1;
            for (std::size_t i = 0; i < options.size(); ++i) {
                float optY = boxPosition.y + boxSize.y * static_cast<float>(i + 1);
                sf::FloatRect optBounds({boxPosition.x, optY}, boxSize);
                if (optBounds.contains(mousePos)) {
                    hoveredIndex = static_cast<int>(i);
                    break;
                }
            }
        } else {
            hoveredIndex = -1;
        }
    }

    // ── Click handling ──────────────────────────────────────────────────
    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button != sf::Mouse::Button::Left) {
            return;
        }

        const sf::Vector2f mousePos(static_cast<float>(mouseEvent->position.x),
                                    static_cast<float>(mouseEvent->position.y));

        // Hit-test main box using stored position/size
        sf::FloatRect mainBounds(boxPosition, boxSize);
        if (mainBounds.contains(mousePos)) {
            expanded = !expanded;
            hoveredIndex = -1;
            return;
        }

        if (expanded) {
            for (std::size_t i = 0; i < options.size(); ++i) {
                float optY = boxPosition.y + boxSize.y * static_cast<float>(i + 1);
                sf::FloatRect optBounds({boxPosition.x, optY}, boxSize);
                if (optBounds.contains(mousePos)) {
                    selectedIndex = i;
                    updateMainText();
                    expanded = false;
                    hoveredIndex = -1;
                    return;
                }
            }
        }

        expanded = false;
        hoveredIndex = -1;
    }
}

void SelectBox::render(sf::RenderWindow& window) {
    window.draw(mainBox);
    window.draw(mainText);
    window.draw(arrowText);

    if (!expanded) {
        return;
    }

    const sf::Color kDefaultBg(44, 52, 74);
    const sf::Color kSelectedBg(120, 90, 235);
    const sf::Color kHoverBg(70, 80, 110);

    for (std::size_t i = 0; i < optionBoxes.size(); ++i) {
        if (static_cast<int>(i) == static_cast<int>(selectedIndex)) {
            optionBoxes[i].setFillColor(kSelectedBg);
        } else if (static_cast<int>(i) == hoveredIndex) {
            optionBoxes[i].setFillColor(kHoverBg);
        } else {
            optionBoxes[i].setFillColor(kDefaultBg);
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
