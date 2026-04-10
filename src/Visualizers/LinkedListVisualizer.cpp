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

    // Perform the actual insertion directly - no traverse for insert
    linkedList.insert(value);

    // Find the index of the newly inserted node (last node)
    int newIndex = 0;
    Node* cur = linkedList.getHead();
    while (cur && cur->next) {
        newIndex++;
        cur = cur->next;
    }

    // Record only the final state with green highlight on new node
    recordStep(-1, "Inserted " + std::to_string(value), newIndex);

    // Jump to the final (only) step
    currentStep = 0;
    elapsedTime = 0.0f;
    isAnimating = false;  // No step-based animation for insert

    // Start drop animation for the new node
    droppingNodeIndex = newIndex;
    dropAnimProgress = 0.0f;
    isDropAnimating = true;

    updateVisualization();
}

void LinkedListVisualizer::updateVisualization(float windowWidth, float windowHeight) {
    visualNodes.clear();
    nodePositions.clear();
    
    if (steps.empty() || !font) return;

    const InsertStep& currentState = steps[currentStep];
    
    const float spacing = 125.0f;
    const float nodeRadius = 35.0f;
    
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

        // Drop animation - new node falls from above with ease-out cubic
        // Drop distance scales with zoom so it looks consistent at any zoom level
        if (isDropAnimating && static_cast<int>(i) == droppingNodeIndex) {
            float easedProgress = 1.0f - std::pow(1.0f - dropAnimProgress, 3.0f);
            float dropOffset = (1.0f - easedProgress) * -150.0f * zoomLevel;
            position.y += dropOffset;
        }

        // Reconnect animation - nodes slide to close the gap after deletion
        if (isReconnecting && reconnectGapIndex >= 0 && static_cast<int>(i) >= reconnectGapIndex) {
            float easedReconnect = 1.0f - std::pow(1.0f - reconnectProgress, 2.0f);  // ease-out quad
            float gapOffset = (1.0f - easedReconnect) * spacing * zoomLevel;  // Start with gap, slide left
            position.x += gapOffset;
        }

        node->setPosition(position);
        nodePositions.push_back(position);

        // Highlight the current node being processed
        if (static_cast<int>(i) == currentState.blinkIndex) {
            // Blink green/orange using sin wave for node about to be deleted
            float blink = std::sin(blinkElapsed * 12.5f);  // Fast oscillation
            if (blink >= 0.0f) {
                node->setFillColor(sf::Color(50, 205, 50));   // Green
            } else {
                node->setFillColor(sf::Color(255, 165, 0));   // Orange
            }
        } else if (static_cast<int>(i) == currentState.successIndex) {
            node->setFillColor(sf::Color(50, 205, 50)); // Lime Green for success/inserted
        } else if (static_cast<int>(i) == currentState.highlightedIndex) {
            node->setFillColor(sf::Color(255, 165, 0)); // Orange for traversing
        } else {
            node->setFillColor(sf::Color(135, 206, 235)); // Light blue
        }

        // Fade-out animation - reduce alpha for the node being deleted
        if (isFadingOut && static_cast<int>(i) == fadeNodeIndex) {
            float easedFade = fadeProgress * fadeProgress;  // ease-in quad (accelerates fade)
            std::uint8_t alpha = static_cast<std::uint8_t>(255 * (1.0f - easedFade));
            node->setAlpha(alpha);
        }

        visualNodes.push_back(std::move(node));
    }
}

void LinkedListVisualizer::recordStep(int highlightedIndex, const std::string& description, int successIndex, int blinkIndex) {
    InsertStep step;
    step.nodeValues = listToVector();
    step.highlightedIndex = highlightedIndex;
    step.successIndex = successIndex;
    step.blinkIndex = blinkIndex;
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
    // Update blink timer (always runs for smooth blinking)
    blinkElapsed += deltaTime;

    // Update drop animation
    if (isDropAnimating) {
        dropAnimProgress += deltaTime * 2.5f;  // ~0.4 seconds for full drop
        if (dropAnimProgress >= 1.0f) {
            dropAnimProgress = 1.0f;
            isDropAnimating = false;
            droppingNodeIndex = -1;
        }
    }

    // Update fade-out animation
    if (isFadingOut) {
        fadeProgress += deltaTime * 2.0f;  // ~0.5 seconds to fully fade
        if (fadeProgress >= 1.0f) {
            fadeProgress = 1.0f;
            isFadingOut = false;

            // Fade done -> advance to the final step (node removed) and start reconnect
            reconnectGapIndex = fadeNodeIndex;
            fadeNodeIndex = -1;
            goToNextStep();  // Now on the step where node is removed

            isReconnecting = true;
            reconnectProgress = 0.0f;
        }
        return;  // Don't process step-based animation during fade
    }

    // Update reconnect animation
    if (isReconnecting) {
        reconnectProgress += deltaTime * 2.5f;  // ~0.4 seconds to slide together
        if (reconnectProgress >= 1.0f) {
            reconnectProgress = 1.0f;
            isReconnecting = false;
            reconnectGapIndex = -1;
            // Now stop animating - delete is fully complete
            isAnimating = false;
        }
        return;  // Don't process step-based animation during reconnect
    }

    // Step-based animation (for traverse in delete, etc.)
    // >>> TRAVERSE SPEED: change the 0.75f below to adjust delay between steps <<<
    // >>> Lower = faster traverse, Higher = slower traverse <<<
    if ((autoRun || isAnimating) && currentStep < static_cast<int>(steps.size()) - 1) {
        // For blink steps, use a longer delay so the blink is visible
        bool isBlinkStep = !steps.empty() && currentStep < static_cast<int>(steps.size())
                           && steps[currentStep].blinkIndex >= 0;
        float stepDelay = isBlinkStep ? 1.5f : 0.75f;  // <<< BLINK: 1.5s, TRAVERSE: 0.75s

        elapsedTime += deltaTime * playbackSpeed;
        
        if (elapsedTime >= stepDelay) {
            // If this was a blink step, transition to fade-out instead of jumping to next step
            if (isBlinkStep) {
                isFadingOut = true;
                fadeProgress = 0.0f;
                fadeNodeIndex = steps[currentStep].blinkIndex;
                elapsedTime = 0.0f;
            } else {
                goToNextStep();
            }
        }
    }
    
    // Stop animating when we reach the last step (for non-delete animations)
    if (isAnimating && !isFadingOut && !isReconnecting
        && currentStep >= static_cast<int>(steps.size()) - 1) {
        isAnimating = false;
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

        // Determine arrow alpha (fade arrows connected to fading node)
        std::uint8_t arrowAlpha = 150;
        if (isFadingOut && fadeNodeIndex >= 0) {
            if (static_cast<int>(i) == fadeNodeIndex || static_cast<int>(i + 1) == static_cast<size_t>(fadeNodeIndex)) {
                float easedFade = fadeProgress * fadeProgress;
                arrowAlpha = static_cast<std::uint8_t>(150 * (1.0f - easedFade));
            }
        }
        
        // Create line connecting nodes
        sf::Vertex line[] = {
            sf::Vertex(start, sf::Color(150, 150, 150, arrowAlpha)),
            sf::Vertex(end, sf::Color(150, 150, 150, arrowAlpha))
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }

    // Render nodes
    for (auto& node : visualNodes) {
        node->render(window);
    }
}
void LinkedListVisualizer::deleteValue(int value) {
    // Clear previous animation state
    steps.clear();
    currentStep = 0;
    nodePositions.clear();

    // First, record the initial state before deletion
    recordStep(-1, "Searching for value " + std::to_string(value));

    // Find the node to delete and track its index
    Node* cur = linkedList.getHead();
    int targetIndex = -1;
    int index = 0;

    // Search for the value in the list - highlight each node as we traverse
    while (cur) {
        recordStep(index, "Checking node at index " + std::to_string(index) + " (value: " + std::to_string(cur->value) + ")");
        if (cur->value == value) {
            targetIndex = index;
            break;
        }
        cur = cur->next;
        index++;
    }

    // If found, blink it green/orange then delete
    if (targetIndex != -1) {
        // Blink step - node will flash green/orange in real-time during this step
        recordStep(-1, "Found " + std::to_string(value) + "! Blinking before removal...", -1, targetIndex);
        
        // Now perform the actual deletion
        linkedList.remove(value);
        
        // Record the final state with the node removed
        recordStep(-1, "Successfully removed " + std::to_string(value));
    } else {
        recordStep(-1, "Value " + std::to_string(value) + " not found in list");
    }

    // Start animation from step 0
    currentStep = 0;
    elapsedTime = 0.0f;
    blinkElapsed = 0.0f;
    isAnimating = true;
    
    // Force immediate visualization update
    updateVisualization();
}
