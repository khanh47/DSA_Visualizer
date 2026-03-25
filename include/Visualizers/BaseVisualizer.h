#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class BaseVisualizer {
public:
    virtual ~BaseVisualizer() = default;
    virtual void processEvents(const sf::Event& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void reset() = 0;
    virtual std::string getProperties() const = 0;

    // Playback controls are optional for each visualizer implementation.
    virtual void setPlaybackSpeed(float speed) { (void)speed; }
    virtual void setAutoRun(bool autoRun) { (void)autoRun; }
    virtual void goToFirstStep() {}
    virtual void goToPreviousStep() {}
    virtual void goToNextStep() {}
    virtual void goToFinalStep() {}
};
