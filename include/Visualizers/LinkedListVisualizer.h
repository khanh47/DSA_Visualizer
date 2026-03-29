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
    LinkedList& list();
    const LinkedList& list() const;

private:
    LinkedList linkedList;
    std::vector<InsertStep> steps;
    int currentStep = 0;
    float playbackSpeed = 1.0f;
    bool autoRun = false;
    float elapsedTime = 0.0f;

    std::vector<std::unique_ptr<UI::VisualNode>> visualNodes;
    std::vector<sf::Vector2f> nodePositions; // Store node positions for drawing arrows
    sf::Font* font = nullptr;

    void updateVisualization(float windowWidth = 800.0f, float windowHeight = 600.0f);
    std::vector<int> listToVector() const;
    void recordStep(int highlightedIndex, const std::string& description);
};
