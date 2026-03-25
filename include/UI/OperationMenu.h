#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

#include "ButtonMenu.h"
#include "OperationCommand.h"
#include "TextBoxGroup.h"

struct OperationMenuItem {
    std::string label;
    OperationType type;
};

class VisualizationScene;

class OperationMenu {
private:
    std::unique_ptr<UI::ButtonMenu> menu;
    UI::TextBoxGroup inputTextBoxes;
    std::vector<OperationMenuItem> menuItems;
    VisualizationScene& scene;
    sf::RectangleShape background;
    sf::Vector2f position;
    sf::Vector2f size;

public:
    explicit OperationMenu(VisualizationScene& scene);

    void processEvents(const sf::Event& event);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    std::string getInputValue(std::size_t index = 0) const;
    std::size_t getInputCount() const;

private:
    float calculateMenuStartX(std::size_t buttonCount, float spacing, float buttonWidth) const;
    void initMenu();
    void initInputBoxes();
};
