#include "LinkedListScene.h"
#include "ResourceManager.h"
#include <iostream>

LinkedListScene::LinkedListScene(SceneManager& sceneManager)
    : VisualizationScene(sceneManager) {
    initializeOperationMenu();
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

void LinkedListScene::onReset() {
    if (visualizer) {
        visualizer->reset();
    }
    displayStatus("Linked list reset.");
}

void LinkedListScene::onPlaybackSpeedChanged(float speed) {
    if (visualizer) {
        visualizer->setPlaybackSpeed(speed);
    }
}

void LinkedListScene::onTogglePlaybackMode(bool autoRun) {
    if (visualizer) {
        visualizer->setAutoRun(autoRun);
    }
    displayStatus(autoRun ? "Playback mode: auto" : "Playback mode: manual");
}

void LinkedListScene::onGoToFirstStep() {
    if (visualizer) {
        visualizer->goToFirstStep();
    }
    displayStatus("Go to first step.");
}

void LinkedListScene::onGoToPreviousStep() {
    if (visualizer) {
        visualizer->goToPreviousStep();
    }
    displayStatus("Go to previous step.");
}

void LinkedListScene::onGoToNextStep() {
    if (visualizer) {
        visualizer->goToNextStep();
    }
    displayStatus("Go to next step.");
}

void LinkedListScene::onGoToFinalStep() {
    if (visualizer) {
        visualizer->goToFinalStep();
    }
    displayStatus("Go to final step.");
}

std::string LinkedListScene::getSceneTitle() const {
    return "Linked List Visualization";
}