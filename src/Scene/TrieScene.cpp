#include "TrieScene.h"
#include "TrieVisualizer.h"
#include "ResourceManager.h"
#include <iostream>

TrieScene::TrieScene(SceneManager& sceneManager)
    : VisualizationScene(sceneManager) {
    initializeOperationMenu();
    visualizer = std::make_unique<TrieVisualizer>();
}

void TrieScene::onInsert(const std::string& value) {
    if (visualizer) {
        static_cast<TrieVisualizer*>(visualizer.get())->insertWord(value);
    }
    displayStatus("Inserted: " + value);
}

void TrieScene::onSearch(const std::string& value) {
    if (visualizer) {
        static_cast<TrieVisualizer*>(visualizer.get())->searchWord(value);
    }
    displayStatus("Searching: " + value);
}

void TrieScene::onDelete(const std::string& value) {
    if (visualizer) {
        static_cast<TrieVisualizer*>(visualizer.get())->removeWord(value);
    }
    displayStatus("Deleted: " + value);
}

void TrieScene::onUpdate(const std::string& key, const std::string& value) {
    if (visualizer) {
        auto* tv = static_cast<TrieVisualizer*>(visualizer.get());
        tv->removeWord(key);
        tv->insertWord(value);
    }
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
