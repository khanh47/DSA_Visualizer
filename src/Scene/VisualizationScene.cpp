#include "VisualizationScene.h"
#include "ResourceManager.h"
#include "OperationCommand.h"
#include "SceneCommand.h"
#include <iostream>

VisualizationScene::VisualizationScene(SceneManager& sceneManager)
    : manager(sceneManager),
      statusText(ResourceManager::getInstance().getFont("Roboto"), "") {
    
    // Initialize back button
    statusText.setPosition({100.0f, 500.0f});
    statusText.setCharacterSize(28);
    statusText.setFillColor(sf::Color(30, 30, 30));
    statusText.setStyle(sf::Text::Bold);
    backButton = std::unique_ptr<UI::Button>(new UI::Button({20.0f, 20.0f}, {220.0f, 60.0f}, sf::Color(100, 149, 237), "Back to menu", 28));
    backButton->setCommand(createPopSceneCommand(manager));
}

void VisualizationScene::initializeOperationPanel() {
    if (!operationPanel) {
        operationPanel = std::make_unique<OperationPanel>(*this);
    }
}

void VisualizationScene::setVisualizer(BaseVisualizer* viz) {
}

void VisualizationScene::displayStatus(const std::string& message) {
    statusText.setString(message);
}

void VisualizationScene::processEvents(const sf::Event& event) {
    if (operationPanel) {
        operationPanel->processEvents(event);
    }

    // Process back button last because it can pop (destroy) this scene.
    backButton->processEvent(event);
}

void VisualizationScene::update(float deltaTime) {
}

void VisualizationScene::render(sf::RenderWindow& window) {
    backButton->render(window);
    if (operationPanel) {
        operationPanel->render(window);
    }

    // text to recognize each scene (can delete later)
    sf::Font &font = ResourceManager::getInstance().getFont("Roboto");
    sf::Text label(font, getSceneTitle(), 30);
    label.setFillColor(sf::Color::Red);
    const sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin({bounds.position.x + bounds.size.x / 2.0f, bounds.position.y});
    label.setPosition({window.getSize().x / 2.0f, 20.0f});

    window.draw(label);
    window.draw(statusText);
}