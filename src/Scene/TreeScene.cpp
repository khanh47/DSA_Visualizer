#include "TreeScene.h"
#include "ResourceManager.h"
#include <iostream>

TreeScene::TreeScene(SceneManager& sceneManager)
    : VisualizationScene(sceneManager) {
    initializeOperationPanel();
    // TODO: Initialize Tree-specific visualization
    // TODO: Create TreeVisualizer and set it via setVisualizer()
}

void TreeScene::onInsert(const std::string& value) {
    // TODO: Insert value into Tree structure
    displayStatus("Inserted: " + value);
}

void TreeScene::onSearch(const std::string& value) {
    // TODO: Search for value in Tree structure
    displayStatus("Searching: " + value);
}

void TreeScene::onDelete(const std::string& value) {
    // TODO: Delete value from Tree structure
    displayStatus("Deleted: " + value);
}

void TreeScene::onUpdate(const std::string& key, const std::string& value) {
    // TODO: Update Tree with given mode (e.g., BFS, DFS)
    displayStatus("Updating " + key + " with: " + value);
}

std::string TreeScene::getSceneTitle() const {
    return "Tree Visualization";
}