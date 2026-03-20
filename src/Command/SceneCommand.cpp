#include "SceneCommand.h"
#include "GraphScene.h"
#include <iostream>

SceneCommand::SceneCommand(SceneManager& manager, SceneFactory factory, const std::string& commandName)
    : sceneManager(manager), sceneFactory(factory), commandName(commandName) {}

void SceneCommand::execute() {
    std::cout << "Execute command: " << commandName << std::endl;
    if (sceneFactory) {
        sceneManager.pushScene(sceneFactory(sceneManager));
    } else {
        // If no factory, assume it's a pop command
        sceneManager.popScene();
    }
}

// Factory implementation for graph scene
std::unique_ptr<BaseScene> createGraphSceneFactory(SceneManager& manager) {
    return std::make_unique<GraphScene>(manager);
}