#pragma once
#include <memory>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "BasePanel.h"
#include "ButtonMenu.h"
#include "SelectBox.h"
#include "TextBoxGroup.h"
#include "OperationCommand.h"

class VisualizationScene;

class OperationPanel : public BasePanel {
private:
    std::unique_ptr<UI::ButtonMenu> operationMenu;
    std::unique_ptr<UI::ButtonMenu> runMenu;
    UI::TextBoxGroup inputTextBoxes;
    std::vector<sf::Text> inputLabels;
    std::unique_ptr<UI::SelectBox> algorithmSelector;
    std::unique_ptr<sf::Text> algorithmSelectorLabel;
    VisualizationScene& scene;
    OperationType type;
    sf::RectangleShape panelBackground;

    float calculateMenuStartX(std::size_t buttonCount, float spacing, float buttonWidth) const;

public:
    OperationPanel(VisualizationScene& vsScene);

    void processEvents(const sf::Event& event);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void get(const OperationType& _type);
    std::string getInputValue(std::size_t index = 0) const;
    std::size_t getInputCount() const;
    std::string getRunAlgorithm() const;
    void initOperationMenu();
    void initInputBoxes();
    void initRunMenu();
};