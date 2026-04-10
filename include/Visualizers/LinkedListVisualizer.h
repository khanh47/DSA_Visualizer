#pragma once

#include "BaseVisualizer.h"
#include "LinkedListStructure.h"
#include "VisualNode.h"

#include <memory>
#include <vector>
#include <queue>

struct InsertStep {
    std::vector<int> nodeValues;
    int highlightedIndex;
    int successIndex = -1;
    int blinkIndex = -1;  // Index of node to blink green/orange before deletion
    std::string description;
};

class LinkedListVisualizer : public BaseVisualizer {
public:
    LinkedListVisualizer();
    ~LinkedListVisualizer() override = default;

    void processEvents(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void reset() override;
    std::string getProperties() const override;

    void setPlaybackSpeed(float speed) override;
    void setAutoRun(bool autoRun) override;
    void goToFirstStep() override;
    void goToPreviousStep() override;
    void goToNextStep() override;
    void goToFinalStep() override;

    void insertValue(int value);
    void deleteValue(int value);
    LinkedList& list();
    const LinkedList& list() const;

private:
    LinkedList linkedList;
    std::vector<InsertStep> steps;
    int currentStep = 0;
    float playbackSpeed = 1.0f;
    bool autoRun = false;
    bool isAnimating = false;  // Auto-plays animation when an operation is triggered
    float elapsedTime = 0.0f;

    // Drop animation for newly inserted nodes
    int droppingNodeIndex = -1;
    float dropAnimProgress = 0.0f;
    bool isDropAnimating = false;

    // Blink animation for node about to be deleted
    float blinkElapsed = 0.0f;  // Accumulates time for blink oscillation

    // Fade-out animation for the deleted node
    bool isFadingOut = false;
    float fadeProgress = 0.0f;
    int fadeNodeIndex = -1;

    // Reconnect animation after node deletion (nodes slide to close the gap)
    bool isReconnecting = false;
    float reconnectProgress = 0.0f;
    int reconnectGapIndex = -1;  // Where the gap was (deleted node's index)

    std::vector<std::unique_ptr<UI::VisualNode>> visualNodes;
    std::vector<sf::Vector2f> nodePositions; // Store node positions for drawing arrows
    sf::Font* font = nullptr;

    void updateVisualization(float windowWidth = 800.0f, float windowHeight = 600.0f);
    std::vector<int> listToVector() const;
    void recordStep(int highlightedIndex, const std::string& description, int successIndex = -1, int blinkIndex = -1);
};
