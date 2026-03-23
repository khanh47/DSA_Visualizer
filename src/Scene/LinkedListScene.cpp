#include "LinkedListScene.h"
#include "ResourceManager.h"
#include <iostream>

LinkedListScene::LinkedListScene(SceneManager& sceneManager)
    : VisualizationScene(sceneManager) {
    // TODO: Initialize LinkedList-specific visualization
    // TODO: Create LinkedListVisualizer and set it via setVisualizer()
}

void LinkedListScene::onInsert(const std::string& value) {
    // TODO: Insert value into LinkedList structure
    displayStatus("Inserted: " + value);
}

void LinkedListScene::onSearch(const std::string& value) {
    // TODO: Search for value in LinkedList structure
    displayStatus("Searching: " + value);
}

void LinkedListScene::onDelete(const std::string& value) {
    // TODO: Delete value from LinkedList structure
    displayStatus("Deleted: " + value);
}

void LinkedListScene::onUpdate(const std::string& key, const std::string& value) {
    // TODO: Update LinkedList with given mode (e.g., BFS, DFS)
    displayStatus("Updating " + key + " with: " + value);
}

std::string LinkedListScene::getSceneTitle() const {
    return "Linked List Visualization";
}