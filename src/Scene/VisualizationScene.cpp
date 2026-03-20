#include "VisualizationScene.h"
#include "ResourceManager.h"
#include "OperationCommand.h"
#include "SceneCommand.h"
#include <iostream>

VisualizationScene::VisualizationScene(SceneManager& sceneManager)
    : manager(sceneManager), visualizer(nullptr),
      statusText(ResourceManager::getInstance().getFont("Roboto"), "") {
    
    // Initialize back button
    backButton = std::unique_ptr<UI::Button>(new UI::Button({20.0f, 80.0f}, {220.0f, 60.0f}, sf::Color(100, 149, 237), "Back to menu", 28));
    backButton->setCommand(createPopSceneCommand(manager));
    
    // TODO: Initialize operation buttons (Insert, Search, Delete, Update)
    // TODO: Set up layout for visualization area, operations panel, properties
}

void VisualizationScene::setVisualizer(BaseVisualizer* viz) {
    visualizer = viz;
}

void VisualizationScene::displayStatus(const std::string& message) {
    statusText.setString(message);
}

void VisualizationScene::processEvents(const sf::Event& event) {
    // Handle back button
    backButton->processEvent(event);
    
    // TODO: Handle operation button clicks
    // Dispatch OperationCommand to onInsert/onSearch/onDelete/onTraverse
}

void VisualizationScene::update(float deltaTime) {
    // TODO: Update operation buttons state
    
    if (visualizer) {
        visualizer->update(deltaTime);
    }
}

void VisualizationScene::render(sf::RenderWindow& window) {
    // Render visualizer
    if (visualizer) {
        visualizer->render(window);
    }
    
    // TODO: Render visualization area
    // TODO: Render operation buttons
    // TODO: Render statusText
    
    // Render back button
    backButton->render(window);
}