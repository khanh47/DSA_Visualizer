#include "HashTableScene.h"
#include "ResourceManager.h"
#include <iostream>

HashTableScene::HashTableScene(SceneManager& sceneManager)
    : VisualizationScene(sceneManager) {
    initializeOperationMenu();
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

void HashTableScene::onReset() {
    if (visualizer) {
        visualizer->reset();
    }
    displayStatus("Hash table reset.");
}

void HashTableScene::onPlaybackSpeedChanged(float speed) {
    if (visualizer) {
        visualizer->setPlaybackSpeed(speed);
    }
}

void HashTableScene::onTogglePlaybackMode(bool autoRun) {
    if (visualizer) {
        visualizer->setAutoRun(autoRun);
    }
    displayStatus(autoRun ? "Playback mode: auto" : "Playback mode: manual");
}

void HashTableScene::onGoToFirstStep() {
    if (visualizer) {
        visualizer->goToFirstStep();
    }
    displayStatus("Go to first step.");
}

void HashTableScene::onGoToPreviousStep() {
    if (visualizer) {
        visualizer->goToPreviousStep();
    }
    displayStatus("Go to previous step.");
}

void HashTableScene::onGoToNextStep() {
    if (visualizer) {
        visualizer->goToNextStep();
    }
    displayStatus("Go to next step.");
}

void HashTableScene::onGoToFinalStep() {
    if (visualizer) {
        visualizer->goToFinalStep();
    }
    displayStatus("Go to final step.");
}

std::string HashTableScene::getSceneTitle() const {
    return "Hash Table Visualization";
}