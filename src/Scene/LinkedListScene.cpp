#include "LinkedListScene.h"
#include "LinkedListVisualizer.h"
#include "ResourceManager.h"
#include <iostream>

LinkedListScene::LinkedListScene(SceneManager& sceneManager)
    : VisualizationScene(sceneManager) {
    initializeOperationMenu();
    
    // Create and set the LinkedList visualizer
    auto linkedListVisualizer = std::make_unique<LinkedListVisualizer>();
    visualizer = std::move(linkedListVisualizer);
}

void LinkedListScene::onInsert(const std::string& value) {
    try {
        int val = std::stoi(value);
        if (visualizer) {
            auto* llVisualizer = dynamic_cast<LinkedListVisualizer*>(visualizer.get());
            if (llVisualizer) {
                llVisualizer->insertValue(val);
            }
        }
    } catch (const std::exception& e) {
    }
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
}

void LinkedListScene::onGoToPreviousStep() {
    if (visualizer) {
        visualizer->goToPreviousStep();
    }
}

void LinkedListScene::onGoToNextStep() {
    if (visualizer) {
        visualizer->goToNextStep();
    }
}

void LinkedListScene::onGoToFinalStep() {
    if (visualizer) {
        visualizer->goToFinalStep();
    }
}

std::string LinkedListScene::getSceneTitle() const {
    return "Linked List Visualization";
}