#include "LinkedListVisualizer.h"
#include "ResourceManager.h"
#include <sstream>

LinkedListVisualizer::LinkedListVisualizer() {
    font = &ResourceManager::getInstance().getFont("Roboto");
}

void LinkedListVisualizer::insertValue(int value) {
    // Record initial state before insert
    steps.clear();
    currentStep = 0;

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
}

void LinkedListVisualizer::updateVisualization() {
    visualNodes.clear();
    
    if (steps.empty() || !font) return;

    const InsertStep& currentState = steps[currentStep];
    
    const float startX = 50.0f;
    const float startY = 200.0f;
    const float spacing = 80.0f;

    for (size_t i = 0; i < currentState.nodeValues.size(); ++i) {
        auto node = std::make_unique<UI::VisualNode>(
            *font, 
            std::to_string(currentState.nodeValues[i]), 
            28.0f
        );

        sf::Vector2f position(startX + i * spacing, startY);
        node->setPosition(position);

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
    (void)event;
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
    updateVisualization();

    // Render nodes
    for (auto& node : visualNodes) {
        node->render(window);
    }

    // Render description text
    if (currentStep < static_cast<int>(steps.size()) && font) {
        sf::Text descText(*font, steps[currentStep].description, 16);
        descText.setFillColor(sf::Color::White);
        descText.setPosition({50.0f, 100.0f});
        window.draw(descText);
    }

    // Render step counter
    if (font) {
        sf::Text stepCounterText(*font, "Step: " + std::to_string(currentStep + 1) + "/" + 
                                  std::to_string(steps.size()), 14);
        stepCounterText.setFillColor(sf::Color(200, 200, 200));
        stepCounterText.setPosition({50.0f, 320.0f});
        window.draw(stepCounterText);
    }
}
