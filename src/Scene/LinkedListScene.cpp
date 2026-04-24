#include "LinkedListScene.h"
#include "LinkedListVisualizer.h"
#include "PseudocodeData.h"
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
                std::string option = operationMenu ? operationMenu->getInsertOption() : "";
                bool atHead = (option == "At Head");

                // Set pseudocode before running the operation
                if (pseudocodePanel) {
                    if (atHead) {
                        pseudocodePanel->setPseudocode(Pseudocode::kLinkedListInsertHeadTitle,
                                                       Pseudocode::kLinkedListInsertHead);
                    } else {
                        pseudocodePanel->setPseudocode(Pseudocode::kLinkedListInsertTailTitle,
                                                       Pseudocode::kLinkedListInsertTail);
                    }
                }

                llVisualizer->insertValue(val, atHead);
            }
        }
    } catch (const std::exception& e) {
    }
}

void LinkedListScene::onSearch(const std::string& value) {
    try {
        int val = std::stoi(value);
        if (visualizer) {
            auto* llVisualizer = dynamic_cast<LinkedListVisualizer*>(visualizer.get());
            if (llVisualizer) {
                if (pseudocodePanel) {
                    pseudocodePanel->setPseudocode(Pseudocode::kLinkedListSearchTitle,
                                                   Pseudocode::kLinkedListSearch);
                }
                llVisualizer->searchValue(val);
            }
        }
    } catch (const std::exception& e) {
        displayStatus("Invalid value: " + value);
    }
}

void LinkedListScene::onDelete(const std::string& value) {
    try {
        int index = std::stoi(value);
        if (visualizer) {
            auto* llVisualizer = dynamic_cast<LinkedListVisualizer*>(visualizer.get());
            if (llVisualizer) {
                if (pseudocodePanel) {
                    pseudocodePanel->setPseudocode(Pseudocode::kLinkedListDeleteTitle,
                                                   Pseudocode::kLinkedListDelete);
                }
                llVisualizer->deleteByIndex(index);
            }
        }
    } catch (const std::exception& e) {
        displayStatus("Invalid index: " + value);
    }
}

void LinkedListScene::onUpdate(const std::string& key, const std::string& value) {
    try {
        int index = std::stoi(key);
        int newVal = std::stoi(value);
        if (visualizer) {
            auto* llVisualizer = dynamic_cast<LinkedListVisualizer*>(visualizer.get());
            if (llVisualizer) {
                if (pseudocodePanel) {
                    pseudocodePanel->setPseudocode(Pseudocode::kLinkedListUpdateTitle,
                                                   Pseudocode::kLinkedListUpdate);
                }
                llVisualizer->updateByIndex(index, newVal);
            }
        }
    } catch (const std::exception& e) {
        displayStatus("Invalid index or value: " + key + ", " + value);
    }
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