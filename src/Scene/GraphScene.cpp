#include "GraphScene.h"
#include "ResourceManager.h"
#include <iostream>

GraphScene::GraphScene(SceneManager& sceneManager)
    : VisualizationScene(sceneManager) {
    // TODO: Initialize graph-specific visualization
    // TODO: Create GraphVisualizer and set it via setVisualizer()
}

void GraphScene::onInsert(const std::string& value) {
    // TODO: Insert value into graph structure
    displayStatus("Inserted: " + value);
}

void GraphScene::onSearch(const std::string& value) {
    // TODO: Search for value in graph structure
    displayStatus("Searching: " + value);
}

void GraphScene::onDelete(const std::string& value) {
    // TODO: Delete value from graph structure
    displayStatus("Deleted: " + value);
}

void GraphScene::onUpdate(const std::string& key, const std::string& value) {
    // TODO: Update graph with given mode (e.g., BFS, DFS)
    displayStatus("Updating " + key + " with: " + value);
}

std::string GraphScene::getSceneTitle() const {
    return "Graph Visualization";
}