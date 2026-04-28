#pragma once
#include "BaseVisualizer.h"
#include "ChainingHashTable.h"
#include <vector>
#include <list>
#include <SFML/Graphics.hpp>

// Snapshot structure to store the table at a specific step
struct HashSnapshot {
    std::vector<std::list<HashNode>> tableState;
    int highlightBucket = -1;
    int highlightNodeIdx = -1;
    int pseudoCodeLine = -1;
};

class HashTableVisualizer : public BaseVisualizer {
private:
    std::unique_ptr<IHashTable> dataStructure;
    sf::Font font;
    
    // Snapshot System
    std::vector<HashSnapshot> history;
    int currentStep = 0;

    // Animation Timer (Only used for the sine wave pulsing effect now)
    float elapsedTime = 0.0f;
    float currentSpeed = 1.0f;

    // Helper draw functions (drawBox now returns its dynamic width)
    float drawBox(sf::RenderWindow& window, float x, float y, const std::string& text, sf::Color bgColor);
    void drawArrow(sf::RenderWindow& window, float x1, float y1, float x2, float y2);
    void renderChaining(sf::RenderWindow& window);

public:
    HashTableVisualizer();
    
    // --- SNAPSHOT CONTROLS ---
    void clearHistory();
    void recordState(int bucket = -1, int nodeIdx = -1, int codeLine = -1);
    void setStep(int stepIndex);

    IHashTable* getData() { return dataStructure.get(); }
    void setMode(HashMode mode);
    
    // --- OVERRIDES ---
    void reset() override;
    void update(float deltaTime) override; 
    void render(sf::RenderWindow& window) override;
    void setPlaybackSpeed(float speed) override;

    // --- UNUSED OVERRIDES ---
    void processEvents(const sf::Event& event) override {}
    void setAutoRun(bool value) override {}
    std::string getProperties() const override { return "Hash Table Visualizer"; }

    int getCurrentPseudocodeLine() const override;
};