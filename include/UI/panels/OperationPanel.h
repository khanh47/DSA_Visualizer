#pragma once
#include <vector>
#include <memory>
#include "ButtonMenu.h"
#include "OperationCommand.h"

class VisualizationScene;

class OperationPanel {
private:
    std::unique_ptr<UI::ButtonMenu> btnMenu;
    VisualizationScene& scene;
    OperationType type;

public:
    OperationPanel(VisualizationScene& vsScene);

    void processEvents(const sf::Event& event);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void get(const OperationType& _type) { type = _type; }
};