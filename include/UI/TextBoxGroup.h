#pragma once

#include <memory>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "TextBox.h"

namespace UI {

class TextBoxGroup {
public:
    void clear();
    void add(std::unique_ptr<TextBox> textBox);
    void processEvent(const sf::Event& event);
    void render(sf::RenderWindow& window);

    std::size_t size() const;
    std::string getText(std::size_t index) const;

private:
    std::vector<std::unique_ptr<TextBox>> textBoxes;
};

} // namespace UI
