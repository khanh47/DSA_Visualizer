#include "OperationPanel.h"
#include "VisualizationScene.h"
#include "ResourceManager.h"
#include "OperationCommand.h"
#include <iostream>

OperationPanel::OperationPanel(VisualizationScene& scene) : scene(scene) {
    btnMenu = std::make_unique<UI::ButtonMenu>();

    const sf::Vector2f buttonSize = {180.0f, 80.0f};
    const float spacing = 300.0f;
    const int buttonCount = 4;

    btnMenu->setLayoutProperties(
        {600.0f, 80.0f},
        buttonSize,
        spacing,
        true,
        sf::Color(100, 149, 237),
        28
    );

    if (scene.getSceneTitle() != "Graph Visualization") {
        btnMenu->addButtonAuto("Insert", createInsertCommand(this));
        btnMenu->addButtonAuto("Delete", createDeleteCommand(this));
        btnMenu->addButtonAuto("Search", createSearchCommand(this));
        btnMenu->addButtonAuto("Update", createUpdateCommand(this));
    } else {
        btnMenu->addButtonAuto("Random", createInsertCommand(this));
        btnMenu->addButtonAuto("Run", createDeleteCommand(this));
    }
}

void OperationPanel::processEvents(const sf::Event& event) {
    if (btnMenu) {
        btnMenu->processEvent(event);
    }
}

void OperationPanel::update(float deltaTime) {
}

void OperationPanel::render(sf::RenderWindow& window) {
    sf::Font &font = ResourceManager::getInstance().getFont("Roboto");

    sf::Text label(font, "Operation menu", 25);
    label.setFillColor(sf::Color::Red);
    label.setPosition({220.0f, 100.0f});

    window.draw(label);
    if (btnMenu) {
        btnMenu->render(window);
    }
}