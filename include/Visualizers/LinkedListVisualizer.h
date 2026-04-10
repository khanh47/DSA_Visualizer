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
    int blinkIndex = -1;       // Index of node to blink green/orange before deletion
    int searchFoundIndex = -1; // Index of node found by search (scale up + color transition)
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
    void searchValue(int value);
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

    // Search found animation: blink phase → scale up + color transition
    bool isSearchBlinking = false;         // Phase 1: blink green/orange
    float searchBlinkElapsed = 0.0f;       // Accumulator for blink oscillation
    float searchBlinkDuration = 0.0f;      // How long the blink phase has lasted
    bool isSearchHighlighting = false;     // Phase 2: scale up + color transition
    float searchHighlightProgress = 0.0f;  // 0→1 for color transition & scale

    std::vector<std::unique_ptr<UI::VisualNode>> visualNodes;
    std::vector<sf::Vector2f> nodePositions;   // Store node positions for drawing arrows
    std::vector<float> nodeRenderedRadii;        // Actual radius of each node (for arrow alignment)
    sf::Font* font = nullptr;

    // Layout constants
    static constexpr float kNodeRadius = 37.5f;
    static constexpr float kSpacing   = 150.0f;

    void updateVisualization(float windowWidth = 800.0f, float windowHeight = 600.0f);
    std::vector<int> listToVector() const;
    void recordStep(int highlightedIndex, const std::string& description, int successIndex = -1, int blinkIndex = -1, int searchFoundIndex = -1);

    // Helpers (refactored from render / updateVisualization)
    void applyNodeColor(UI::VisualNode& node, int index, const InsertStep& step, float& renderedRadius);
    void drawArrow(sf::RenderWindow& window, size_t fromIdx, size_t toIdx);
    int buildTraverseSteps(int value);  // shared traverse logic for delete & search
};
