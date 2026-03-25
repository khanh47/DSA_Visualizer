#include "KruskalScene.h"
#include "KruskalVisualizer.h"
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
    displayStatus("Generated random graph.");
}

void KruskalScene::onRun() {
    displayStatus("Running kruskal algorithm.");
}

void KruskalScene::onReset() {
    if (visualizer) {
        visualizer->reset();
    }
    displayStatus("Kruskal reset.");
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
    displayStatus(autoRun ? "Playback mode: auto" : "Playback mode: manual");
}

void KruskalScene::onGoToFirstStep() {
    if (visualizer) {
        visualizer->goToFirstStep();
    }
    displayStatus("Go to first step.");
}

void KruskalScene::onGoToPreviousStep() {
    if (visualizer) {
        visualizer->goToPreviousStep();
    }
    displayStatus("Go to previous step.");
}

void KruskalScene::onGoToNextStep() {
    if (visualizer) {
        visualizer->goToNextStep();
    }
    displayStatus("Go to next step.");
}

void KruskalScene::onGoToFinalStep() {
    if (visualizer) {
        visualizer->goToFinalStep();
    }
    displayStatus("Go to final step.");
}

std::string KruskalScene::getSceneTitle() const {
    return "Kruskal Visualization";
}