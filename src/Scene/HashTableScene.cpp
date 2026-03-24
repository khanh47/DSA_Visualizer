#include "HashTableScene.h"
#include "ResourceManager.h"
#include <iostream>

HashTableScene::HashTableScene(SceneManager& sceneManager)
    : VisualizationScene(sceneManager) {
    initializeOperationPanel();
    // TODO: Initialize HashTable-specific visualization
    // TODO: Create HashTableVisualizer and set it via setVisualizer()
}

void HashTableScene::onInsert(const std::string& value) {
    // TODO: Insert value into HashTable structure
    displayStatus("Inserted: " + value);
}

void HashTableScene::onSearch(const std::string& value) {
    // TODO: Search for value in HashTable structure
    displayStatus("Searching: " + value);
}

void HashTableScene::onDelete(const std::string& value) {
    // TODO: Delete value from HashTable structure
    displayStatus("Deleted: " + value);
}

void HashTableScene::onUpdate(const std::string& key, const std::string& value) {
    // TODO: Update HashTable with given mode (e.g., BFS, DFS)
    displayStatus("Updating " + key + " with: " + value);
}

std::string HashTableScene::getSceneTitle() const {
    return "Hash Table Visualization";
}