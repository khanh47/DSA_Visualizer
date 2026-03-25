#pragma once

#include <functional>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Button.h"
#include "PlaybackSpeed.h"

namespace UI {

class PlaybackControlWidget {
public:
    PlaybackControlWidget();

    void setOnSpeedChanged(std::function<void(float)> callback);
    void setOnModeToggled(std::function<void(bool)> callback);
    void setOnFirstStep(std::function<void()> callback);
    void setOnPreviousStep(std::function<void()> callback);
    void setOnNextStep(std::function<void()> callback);
    void setOnFinalStep(std::function<void()> callback);

    void setAutoRunMode(bool enabled);
    bool isAutoRunMode() const;

    float getPlaybackSpeed() const;
    void setPlaybackSpeed(float speed);

    void processEvent(const sf::Event& event);
    void layout(const sf::Vector2u& windowSize);
    void render(sf::RenderWindow& window);

private:
    void rebuildModeButton();
    void updateSpeedFromMouse(float mouseX);
    void syncSpeedVisuals();

    sf::RectangleShape bottomBar;
    sf::RectangleShape speedTrack;
    sf::RectangleShape speedFill;
    sf::RectangleShape speedKnob;
    sf::Text speedText;

    PlaybackSpeed playbackSpeed;
    bool speedDragging = false;
    bool autoRunMode = false;

    std::unique_ptr<UI::Button> modeButton;
    std::unique_ptr<UI::Button> firstStepButton;
    std::unique_ptr<UI::Button> prevStepButton;
    std::unique_ptr<UI::Button> nextStepButton;
    std::unique_ptr<UI::Button> finalStepButton;

    sf::Vector2f modeButtonPosition;

    std::function<void(float)> onSpeedChanged;
    std::function<void(bool)> onModeToggled;
    std::function<void()> onFirstStep;
    std::function<void()> onPreviousStep;
    std::function<void()> onNextStep;
    std::function<void()> onFinalStep;
};

} // namespace UI
