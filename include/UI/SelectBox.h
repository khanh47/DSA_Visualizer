#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

namespace UI {

class SelectBox {
public:
    SelectBox(const sf::Vector2f& position,
              const sf::Vector2f& size,
              const std::vector<std::string>& options,
              unsigned int charSize = 20);

    void processEvent(const sf::Event& event);
    void render(sf::RenderWindow& window);

    std::string getSelected() const;
    void setSelected(std::size_t index);

private:
    void updateMainText();

    sf::ConvexShape mainBox;         // rounded main box
    sf::Text mainText;
    sf::Text arrowText;

    sf::Vector2f boxPosition;
    sf::Vector2f boxSize;
    static constexpr float kCornerRadius = 8.0f;

    std::vector<std::string> options;
    std::vector<sf::ConvexShape> optionBoxes;  // rounded option boxes
    std::vector<sf::Text> optionTexts;

    bool expanded;
    std::size_t selectedIndex;
    int hoveredIndex = -1;  // index of option currently hovered, -1 = none
};

} // namespace UI
