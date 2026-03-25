#include "KruskalVisualizer.h"
#include <algorithm>
#include "ResourceManager.h"

KruskalVisualizer::KruskalVisualizer() {
    sf::Font& font = ResourceManager::getInstance().getFont("Roboto");
}

void KruskalVisualizer::reset() {
    graph.clear();
    autoRun = false;
    playbackSpeed = 1.0f;
}

std::string KruskalVisualizer::getProperties() const {
    return "Kruskal visualizer";
}

void KruskalVisualizer::setPlaybackSpeed(float speed) {
    playbackSpeed = std::max(0.1f, speed);
}

void KruskalVisualizer::setAutoRun(bool value) {
    autoRun = value;
}

void KruskalVisualizer::goToFirstStep() {
}

void KruskalVisualizer::goToPreviousStep() {
}

void KruskalVisualizer::goToNextStep() {
}

void KruskalVisualizer::goToFinalStep() {
}

KruskalDataStructure& KruskalVisualizer::data() {
    return graph;
}

const KruskalDataStructure& KruskalVisualizer::data() const {
    return graph;
}

void KruskalVisualizer::processEvents(const sf::Event& event) {
}

void KruskalVisualizer::update(float deltaTime) {
}

void KruskalVisualizer::render(sf::RenderWindow& window) {
}