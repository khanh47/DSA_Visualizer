#include "SceneManager.h"
#include "BaseScene.h"
#include "MenuScene.h"
// include the other 4 visualizers here once you make them

SceneManager::SceneManager() {
}

SceneManager::~SceneManager() {
    scenes.clear();
}

void SceneManager::pushScene(std::unique_ptr<BaseScene> newScene) {
    scenes.push_back(std::move(newScene));
}

void SceneManager::popScene() {
    if (!scenes.empty()) {
        scenes.pop_back();
    }
}

void SceneManager::processEvents(const sf::Event& event) {
    if (!scenes.empty()) {
        scenes.back()->processEvents(event);
    }
}

void SceneManager::update(float deltaTime) {
    if (!scenes.empty()) {
        scenes.back()->update(deltaTime);
    }
}

void SceneManager::render(sf::RenderWindow& window) {
    if (!scenes.empty()) {
        scenes.back()->render(window);
    }
}
