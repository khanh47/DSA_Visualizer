#pragma once
#include "BaseVisualizer.h" 
#include "IHashTable.h"
#include "ChainingHashTable.h"
#include <memory>
#include <SFML/Graphics.hpp>

class HashTableVisualizer : public BaseVisualizer {
private:
    std::unique_ptr<IHashTable> dataStructure;
    sf::Font font;

    float elapsedTime = 0.0f;    
    float currentSpeed = 1.0f;
    float animationTimer = 0.0f;

    // Draw Helpers
    void drawBox(sf::RenderWindow& window, float x, float y, const std::string& text, sf::Color bgColor);
    void drawArrow(sf::RenderWindow& window, float startX, float startY, float endX, float endY);
    
    // Specific Renderers for different modes
    void renderChaining(sf::RenderWindow& window);
    // void renderLinearProbing(sf::RenderWindow& window); // Add later

public:
    HashTableVisualizer();

    std::string getProperties() const override; 

    void setPlaybackSpeed(float speed) override;

    void triggerAnimation(); 
    
    void setMode(HashMode mode); // Switch between chaining, probing, etc.
    IHashTable* getData() { return dataStructure.get(); }

    void reset() override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void processEvents(const sf::Event& event) override;

    // Required by BaseVisualizer
    void setAutoRun(bool value) override {}
    void goToFirstStep() override {}
    void goToPreviousStep() override {}
    void goToNextStep() override {}
    void goToFinalStep() override {}
};