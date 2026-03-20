#include "MenuScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GraphScene.h"
#include "SceneCommand.h"
#include <iostream>

MenuScene::MenuScene(SceneManager& sceneManager) : manager(sceneManager) {
    // Initialize UI button
    startButton = std::unique_ptr<UI::Button>(new UI::Button({20.0f, 80.0f}, {220.0f, 60.0f}, sf::Color(100, 149, 237), "DSA Visualization", 28));
    startButton->setCommand(createGraphSceneCommand(manager));
}

void MenuScene::processEvents(const sf::Event& event) {
    startButton->processEvent(event);
}

void MenuScene::update(float deltaTime) {
}

void MenuScene::render(sf::RenderWindow& window) {
    sf::Font &font = ResourceManager::getInstance().getFont("Roboto");

    sf::Text label("Main Menu", font, 24);
    label.setFillColor(sf::Color::Red);
    label.setPosition({20.0f, 20.0f});

    window.draw(label);
    startButton->render(window);
}