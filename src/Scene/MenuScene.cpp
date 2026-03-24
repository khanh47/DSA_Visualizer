#include "MenuScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GraphScene.h"
#include "SceneCommand.h"
#include <iostream>

MenuScene::MenuScene(SceneManager& sceneManager) : manager(sceneManager) {
    btnMenu = std::make_unique<UI::ButtonMenu>();

    const float windowWidth = 1920.0f;
    const float windowHeight = 1080.0f;
    const sf::Vector2f buttonSize = {220.0f, 220.0f};
    const float spacing = 400.0f;
    const int buttonCount = 4;
    const float totalRowWidth = (buttonCount - 1) * spacing + buttonSize.x;
    const float centeredX = (windowWidth - totalRowWidth) / 2.0f;
    const float centeredY = (windowHeight - buttonSize.y) / 2.0f;

    btnMenu->setLayoutProperties(
        {centeredX, centeredY}, // start position
        buttonSize, // button size
        spacing, // spacing between button
        true, // is horizontal
        sf::Color(100, 149, 237), // color of the button
        28 // size of text
    );

    btnMenu->addButtonAuto("Linked List", createLinkedListSceneCommand(manager));
    btnMenu->addButtonAuto("Hash Table", createHashTableSceneCommand(manager));
    btnMenu->addButtonAuto("Tree", createTreeSceneCommand(manager));
    btnMenu->addButtonAuto("Graph", createGraphSceneCommand(manager));
}

void MenuScene::processEvents(const sf::Event& event) {
    if (btnMenu) {
        btnMenu->processEvent(event);
    }
}

void MenuScene::update(float deltaTime) {
}

void MenuScene::render(sf::RenderWindow& window) {
    sf::Font &font = ResourceManager::getInstance().getFont("Roboto");

    sf::Text label(font, "Data Structures Visualizer", 30);
    label.setFillColor(sf::Color::Red);
    const sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin({bounds.position.x + bounds.size.x / 2.0f, bounds.position.y});
    label.setPosition({window.getSize().x / 2.0f, 20.0f});

    window.draw(label);
    if (btnMenu) {
        btnMenu->render(window);
    }
}