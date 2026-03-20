#include "GraphScene.h"
#include "ResourceManager.h"
#include "SceneCommand.h"
#include <iostream>

GraphScene::GraphScene(SceneManager& sceneManager) : manager(sceneManager) {
    backButton = std::unique_ptr<UI::Button>(new UI::Button({20.0f, 80.0f}, {220.0f, 60.0f}, sf::Color(100, 149, 237), "Back to menu", 28));
    backButton->setCommand(createPopSceneCommand(manager));
}

void GraphScene::processEvents(const sf::Event& event) {
    backButton->processEvent(event);
}


void GraphScene::update(float deltaTime) {
}

void GraphScene::render(sf::RenderWindow& window) {
    sf::Font &font = ResourceManager::getInstance().getFont("Roboto");

    sf::Text label("Graph Visualization", font, 24);
    label.setFillColor(sf::Color::Red);
    label.setPosition({20.0f, 20.0f});

    window.draw(label);
    backButton->render(window);
}