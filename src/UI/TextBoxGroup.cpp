#include "TextBoxGroup.h"

namespace UI {

void TextBoxGroup::clear() {
    textBoxes.clear();
}

void TextBoxGroup::add(std::unique_ptr<TextBox> textBox) {
    if (!textBox) {
        return;
    }
    textBoxes.push_back(std::move(textBox));
}

void TextBoxGroup::processEvent(const sf::Event& event) {
    for (const auto& textBox : textBoxes) {
        if (textBox) {
            textBox->processEvent(event);
        }
    }
}

void TextBoxGroup::render(sf::RenderWindow& window) {
    for (const auto& textBox : textBoxes) {
        if (textBox) {
            textBox->render(window);
        }
    }
}

std::size_t TextBoxGroup::size() const {
    return textBoxes.size();
}

std::string TextBoxGroup::getText(std::size_t index) const {
    if (index >= textBoxes.size() || !textBoxes[index]) {
        return "";
    }

    return textBoxes[index]->getText();
}

} // namespace UI
