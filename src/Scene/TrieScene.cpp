#include "TrieScene.h"
#include "ResourceManager.h"
#include <iostream>

TrieScene::TrieScene(SceneManager& sceneManager)
    : VisualizationScene(sceneManager) {
    initializeOperationMenu();
    // TODO: Initialize Trie-specific visualization.
    // TODO: Create TrieVisualizer and set it via setVisualizer().
}

void TrieScene::onInsert(const std::string& value) {
    // TODO: Insert value into Trie structure.
    displayStatus("Inserted: " + value);
}

void TrieScene::onSearch(const std::string& value) {
    // TODO: Search for value in Trie structure.
    displayStatus("Searching: " + value);
}

void TrieScene::onDelete(const std::string& value) {
    // TODO: Delete value from Trie structure.
    displayStatus("Deleted: " + value);
}

void TrieScene::onUpdate(const std::string& key, const std::string& value) {
    // TODO: Update Trie node/value mapping.
    displayStatus("Updating " + key + " with: " + value);
}

void TrieScene::onReset() {
    if (visualizer) {
        visualizer->reset();
    }
    displayStatus("Trie reset.");
}

void TrieScene::onPlaybackSpeedChanged(float speed) {
    if (visualizer) {
        visualizer->setPlaybackSpeed(speed);
    }
}

void TrieScene::onTogglePlaybackMode(bool autoRun) {
    if (visualizer) {
        visualizer->setAutoRun(autoRun);
    }
    displayStatus(autoRun ? "Playback mode: auto" : "Playback mode: manual");
}

void TrieScene::onGoToFirstStep() {
    if (visualizer) {
        visualizer->goToFirstStep();
    }
    displayStatus("Go to first step.");
}

void TrieScene::onGoToPreviousStep() {
    if (visualizer) {
        visualizer->goToPreviousStep();
    }
    displayStatus("Go to previous step.");
}

void TrieScene::onGoToNextStep() {
    if (visualizer) {
        visualizer->goToNextStep();
    }
    displayStatus("Go to next step.");
}

void TrieScene::onGoToFinalStep() {
    if (visualizer) {
        visualizer->goToFinalStep();
    }
    displayStatus("Go to final step.");
}

std::string TrieScene::getSceneTitle() const {
    return "Trie Visualization";
}