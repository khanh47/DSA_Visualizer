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

    // Zoom and pan helpers
    float getZoomLevel() const { return zoomLevel; }
    sf::Vector2f getPanOffset() const { return panOffset; }
    void resetZoomPan() { zoomLevel = 1.0f; panOffset = {0.0f, 0.0f}; }
    void setDragRegion(float topY, float bottomY);

protected:
    float zoomLevel = 1.0f;
    sf::Vector2f panOffset = {0.0f, 0.0f};
    bool isDragging = false;
    sf::Vector2f lastMousePos = {0.0f, 0.0f};
    float dragRegionTopY = 0.0f;
    float dragRegionBottomY = 1.0e9f;

    void handleZoomPanEvents(const sf::Event& event);
    bool isInsideDragRegion(const sf::Vector2f& position) const;
};
