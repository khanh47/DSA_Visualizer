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

    sf::RectangleShape mainBox;
    sf::Text mainText;
    sf::Text arrowText;

    std::vector<std::string> options;
    std::vector<sf::RectangleShape> optionBoxes;
    std::vector<sf::Text> optionTexts;

    bool expanded;
    std::size_t selectedIndex;
};

} // namespace UI
