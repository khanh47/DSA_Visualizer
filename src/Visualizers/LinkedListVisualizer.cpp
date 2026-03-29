#include "LinkedListVisualizer.h"
#include "ResourceManager.h"
#include <sstream>
#include <cmath>
#include <algorithm>

LinkedListVisualizer::LinkedListVisualizer() {
    font = &ResourceManager::getInstance().getFont("Roboto");
}

void LinkedListVisualizer::insertValue(int value) {
    // Clear only steps for new animation, keep the linked list data
    steps.clear();
    currentStep = 0;
    nodePositions.clear();

    Node* cur = linkedList.getHead();
    int index = 0;

    recordStep(-1, "Starting insert of " + std::to_string(value));

    // Traverse to the end
    while (cur && cur->next) {
        recordStep(index, "Traversing to find insertion point");
        cur = cur->next;
        index++;
    }

    // Record before actual insertion
    recordStep(index, "Found insertion point, inserting " + std::to_string(value));

    // Perform the actual insertion
    linkedList.insert(value);

    // Record final state
    recordStep(index + 1, "Inserted " + std::to_string(value));
    
    // Auto-move to final step to show result immediately
    currentStep = steps.size() - 1;
}

void LinkedListVisualizer::updateVisualization(float windowWidth, float windowHeight) {
    visualNodes.clear();
    nodePositions.clear();
    
    if (steps.empty() || !font) return;

    const InsertStep& currentState = steps[currentStep];
    
    const float spacing = 80.0f;
    const float nodeRadius = 28.0f;
    
    // Calculate total width needed for all nodes
    float totalWidth = currentState.nodeValues.size() * spacing;
    
    // Start position centered horizontally
    float startX = (windowWidth - totalWidth) / 2.0f + nodeRadius;
    float startY = windowHeight / 2.0f; // Center vertically

    for (size_t i = 0; i < currentState.nodeValues.size(); ++i) {
        auto node = std::make_unique<UI::VisualNode>(
            *font, 
            std::to_string(currentState.nodeValues[i]), 
            nodeRadius * zoomLevel
        );

        // Calculate base position
        float posX = startX + i * spacing;
        float posY = startY;
        
        // Center zoom around screen center
        float centerX = windowWidth / 2.0f;
        float centerY = windowHeight / 2.0f;
        
        // Apply zoom transformation
        float zoomedX = centerX + (posX - centerX) * zoomLevel;
        float zoomedY = centerY + (posY - centerY) * zoomLevel;
        
        // Apply pan
        sf::Vector2f position(zoomedX + panOffset.x, zoomedY + panOffset.y);
        node->setPosition(position);
        nodePositions.push_back(position);

        // Highlight the current node being processed
        if (static_cast<int>(i) == currentState.highlightedIndex) {
            node->setFillColor(sf::Color(255, 165, 0)); // Orange for highlighted
        } else {
            node->setFillColor(sf::Color(57, 74, 111)); // Default blue
        }

        visualNodes.push_back(std::move(node));
    }
}

void LinkedListVisualizer::recordStep(int highlightedIndex, const std::string& description) {
    InsertStep step;
    step.nodeValues = listToVector();
    step.highlightedIndex = highlightedIndex;
    step.description = description;
    steps.push_back(step);
}

std::vector<int> LinkedListVisualizer::listToVector() const {
    std::vector<int> result;
    Node* cur = linkedList.getHead();
    while (cur) {
        result.push_back(cur->value);
        cur = cur->next;
    }
    return result;
}

void LinkedListVisualizer::reset() {
    linkedList = LinkedList();
    steps.clear();
    currentStep = 0;
    elapsedTime = 0.0f;
    visualNodes.clear();
}

std::string LinkedListVisualizer::getProperties() const {
    std::ostringstream oss;
    oss << "Linked List Visualizer\n";
    oss << "Current Step: " << (currentStep + 1) << "/" << steps.size() << "\n";
    if (currentStep < static_cast<int>(steps.size())) {
        oss << "Status: " << steps[currentStep].description;
    }
    return oss.str();
}

void LinkedListVisualizer::setPlaybackSpeed(float speed) {
    playbackSpeed = std::max(0.1f, speed);
}

void LinkedListVisualizer::setAutoRun(bool value) {
    autoRun = value;
    if (!autoRun) {
        elapsedTime = 0.0f;
    }
}

void LinkedListVisualizer::goToFirstStep() {
    currentStep = 0;
    elapsedTime = 0.0f;
    updateVisualization();
}

void LinkedListVisualizer::goToPreviousStep() {
    if (currentStep > 0) {
        currentStep--;
        elapsedTime = 0.0f;
        updateVisualization();
    }
}

void LinkedListVisualizer::goToNextStep() {
    if (currentStep < static_cast<int>(steps.size()) - 1) {
        currentStep++;
        elapsedTime = 0.0f;
        updateVisualization();
    }
}

void LinkedListVisualizer::goToFinalStep() {
    if (!steps.empty()) {
        currentStep = steps.size() - 1;
        elapsedTime = 0.0f;
        updateVisualization();
    }
}

LinkedList& LinkedListVisualizer::list() {
    return linkedList;
}

const LinkedList& LinkedListVisualizer::list() const {
    return linkedList;
}

void LinkedListVisualizer::processEvents(const sf::Event& event) {
    handleZoomPanEvents(event);
}

void LinkedListVisualizer::update(float deltaTime) {
    if (autoRun && currentStep < static_cast<int>(steps.size()) - 1) {
        elapsedTime += deltaTime * playbackSpeed;
        
        // Move to next step after 1 second at speed 1.0
        if (elapsedTime >= 1.0f) {
            goToNextStep();
        }
    }
}

void LinkedListVisualizer::render(sf::RenderWindow& window) {
    // Get window size for centering
    sf::Vector2u windowSize = window.getSize();
    float windowWidth = static_cast<float>(windowSize.x);
    float windowHeight = static_cast<float>(windowSize.y);
    
    updateVisualization(windowWidth, windowHeight);

    // Draw arrows between nodes
    for (size_t i = 0; i + 1 < nodePositions.size(); ++i) {
        sf::Vector2f start = nodePositions[i];
        sf::Vector2f end = nodePositions[i + 1];
        
        // Create line connecting nodes
        sf::Vertex line[] = {
            sf::Vertex(start, sf::Color(150, 150, 150)),
            sf::Vertex(end, sf::Color(150, 150, 150))
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }

    // Render nodes
    for (auto& node : visualNodes) {
        node->render(window);
    }
}
