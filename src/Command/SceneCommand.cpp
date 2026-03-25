#include "SceneCommand.h"
#include "LinkedListScene.h"
#include "HashTableScene.h"
#include "TrieScene.h"
#include "KruskalScene.h"
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

std::unique_ptr<BaseScene> createTrieSceneFactory(SceneManager& manager) {
    return std::make_unique<TrieScene>(manager);
}

std::unique_ptr<BaseScene> createKruskalSceneFactory(SceneManager& manager) {
    return std::make_unique<KruskalScene>(manager);
}