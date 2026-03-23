#include "SceneCommand.h"
#include "LinkedListScene.h"
#include "HashTableScene.h"
#include "TreeScene.h"
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

// Factory implementation for visualize scene
std::unique_ptr<BaseScene> createLinkedListSceneFactory(SceneManager& manager) {
    return std::make_unique<LinkedListScene>(manager);
}

std::unique_ptr<BaseScene> createHashTableSceneFactory(SceneManager& manager) {
    return std::make_unique<HashTableScene>(manager);
}

std::unique_ptr<BaseScene> createTreeSceneFactory(SceneManager& manager) {
    return std::make_unique<TreeScene>(manager);
}

std::unique_ptr<BaseScene> createGraphSceneFactory(SceneManager& manager) {
    return std::make_unique<GraphScene>(manager);
}