#include "KruskalScene.h"
#include "KruskalVisualizer.h"
#include "PseudocodeData.h"
#include "ResourceManager.h"
#include <algorithm>
#include <cctype>
#include <iostream>

KruskalScene::KruskalScene(SceneManager& sceneManager)
    : VisualizationScene(sceneManager) {
    setVisualizer(new KruskalVisualizer());
    initializeOperationMenu();
}

void KruskalScene::onRandom() {
    if (visualizer) {
        auto* kruskalVisualizer = dynamic_cast<KruskalVisualizer*>(visualizer.get());
        if (kruskalVisualizer) {
            kruskalVisualizer->random();
        }
    }
}

void KruskalScene::onRun() {
    if (pseudocodePanel) {
        pseudocodePanel->setPseudocode(Pseudocode::kKruskalTitle, Pseudocode::kKruskalMain);
    }

    if (visualizer) {
        auto* kruskalVisualizer = dynamic_cast<KruskalVisualizer*>(visualizer.get());
        if (kruskalVisualizer) {
            kruskalVisualizer->run();
        }
    }
}

void KruskalScene::onReset() {
    if (visualizer) {
        visualizer->reset();
    }
}

void KruskalScene::onPlaybackSpeedChanged(float speed) {
    if (visualizer) {
        visualizer->setPlaybackSpeed(speed);
    }
}

void KruskalScene::onTogglePlaybackMode(bool autoRun) {
    if (visualizer) {
        visualizer->setAutoRun(autoRun);
    }
}

void KruskalScene::onGoToFirstStep() {
    if (visualizer) {
        visualizer->goToFirstStep();
    }
}

void KruskalScene::onGoToPreviousStep() {
    if (visualizer) {
        visualizer->goToPreviousStep();
    }
}

void KruskalScene::onGoToNextStep() {
    if (visualizer) {
        visualizer->goToNextStep();
    }
}

void KruskalScene::onGoToFinalStep() {
    if (visualizer) {
        visualizer->goToFinalStep();
    }
}

std::string KruskalScene::getSceneTitle() const {
    return "Kruskal Visualization";
}