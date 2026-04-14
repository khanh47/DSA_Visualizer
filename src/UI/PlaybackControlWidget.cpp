#include "PlaybackControlWidget.h"

#include <algorithm>

#include "PlaybackCommand.h"
#include "ResourceManager.h"

namespace UI {

PlaybackControlWidget::PlaybackControlWidget()
    : speedText(ResourceManager::getInstance().getFont("Roboto"), "1.00x", 20),
      playbackSpeed(0.25f, 3.0f, 1.0f) {
    bottomBar.setFillColor(sf::Color(8, 28, 62));

    speedTrack.setFillColor(sf::Color(95, 105, 122));
    speedFill.setFillColor(sf::Color(40, 221, 255));
    speedKnob.setRadius(7.0f);
    speedKnob.setOrigin({7.0f, 7.0f});
    speedKnob.setFillColor(sf::Color(238, 242, 248));

    speedText.setFillColor(sf::Color(242, 244, 250));
    speedText.setString(playbackSpeed.toLabel());

    modeButtonPosition = {470.0f, 1020.0f};
    rebuildModeButton();

    firstStepButton = std::make_unique<UI::Button>(
        sf::Vector2f(1260.0f, 1020.0f),
        sf::Vector2f(46.0f, 36.0f),
        sf::Color(34, 43, 62),
        "|<",
        18
    );
    firstStepButton->setCommand(createPlaybackCommand([this]() {
        if (onFirstStep) {
            onFirstStep();
        }
    }));

    prevStepButton = std::make_unique<UI::Button>(
        sf::Vector2f(1316.0f, 1020.0f),
        sf::Vector2f(46.0f, 36.0f),
        sf::Color(34, 43, 62),
        "<",
        18
    );
    prevStepButton->setCommand(createPlaybackCommand([this]() {
        if (onPreviousStep) {
            onPreviousStep();
        }
    }));

    nextStepButton = std::make_unique<UI::Button>(
        sf::Vector2f(1372.0f, 1020.0f),
        sf::Vector2f(46.0f, 36.0f),
        sf::Color(34, 43, 62),
        ">",
        18
    );
    nextStepButton->setCommand(createPlaybackCommand([this]() {
        if (onNextStep) {
            onNextStep();
        }
    }));

    finalStepButton = std::make_unique<UI::Button>(
        sf::Vector2f(1428.0f, 1020.0f),
        sf::Vector2f(46.0f, 36.0f),
        sf::Color(34, 43, 62),
        ">|",
        18
    );
    finalStepButton->setCommand(createPlaybackCommand([this]() {
        if (onFinalStep) {
            onFinalStep();
        }
    }));
}

void PlaybackControlWidget::setOnSpeedChanged(std::function<void(float)> callback) {
    onSpeedChanged = std::move(callback);
}

void PlaybackControlWidget::setOnModeToggled(std::function<void(bool)> callback) {
    onModeToggled = std::move(callback);
}

void PlaybackControlWidget::setOnFirstStep(std::function<void()> callback) {
    onFirstStep = std::move(callback);
}

void PlaybackControlWidget::setOnPreviousStep(std::function<void()> callback) {
    onPreviousStep = std::move(callback);
}

void PlaybackControlWidget::setOnNextStep(std::function<void()> callback) {
    onNextStep = std::move(callback);
}

void PlaybackControlWidget::setOnFinalStep(std::function<void()> callback) {
    onFinalStep = std::move(callback);
}

void PlaybackControlWidget::setAutoRunMode(bool enabled) {
    if (autoRunMode == enabled) {
        return;
    }

    autoRunMode = enabled;
    rebuildModeButton();
}

bool PlaybackControlWidget::isAutoRunMode() const {
    return autoRunMode;
}

float PlaybackControlWidget::getPlaybackSpeed() const {
    return playbackSpeed.getValue();
}

void PlaybackControlWidget::setPlaybackSpeed(float speed) {
    playbackSpeed.setValue(speed);
    speedText.setString(playbackSpeed.toLabel());
    syncSpeedVisuals();
}

void PlaybackControlWidget::processEvent(const sf::Event& event) {
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            const sf::Vector2f mousePos(static_cast<float>(mousePressed->position.x),
                                        static_cast<float>(mousePressed->position.y));

            if (speedTrack.getGlobalBounds().contains(mousePos) || speedKnob.getGlobalBounds().contains(mousePos)) {
                speedDragging = true;
                updateSpeedFromMouse(mousePos.x);
            }
        }
    }

    if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        if (speedDragging) {
            updateSpeedFromMouse(static_cast<float>(mouseMoved->position.x));
        }
    }

    if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == sf::Mouse::Button::Left && speedDragging) {
            speedDragging = false;
            updateSpeedFromMouse(static_cast<float>(mouseReleased->position.x));
        }
    }

    if (modeButton) {
        modeButton->processEvent(event);
    }
    if (firstStepButton) {
        firstStepButton->processEvent(event);
    }
    if (prevStepButton) {
        prevStepButton->processEvent(event);
    }
    if (nextStepButton) {
        nextStepButton->processEvent(event);
    }
    if (finalStepButton) {
        finalStepButton->processEvent(event);
    }
}

void PlaybackControlWidget::layout(const sf::Vector2u& windowSize) {
    const float width = static_cast<float>(windowSize.x);
    const float height = static_cast<float>(windowSize.y);
    const float barHeight = std::clamp(height * 0.07f, 56.0f, 76.0f);
    const float barY = height - barHeight;

    bottomBar.setPosition({0.0f, barY});
    bottomBar.setSize({width, barHeight});

    // Vertical center for all controls.
    const float centerY = barY + (barHeight - 36.0f) * 0.5f;

    // Constants for a stable 3-zone layout (left speed, center nav, right mode).
    const float navButtonWidth = std::clamp(width * 0.08f, 32.0f, 46.0f);
    const float navButtonHeight = std::clamp(height * 0.035f, 30.0f, 36.0f);
    const float navGap = std::clamp(width * 0.012f, 4.0f, 10.0f);
    const float navGroupWidth = 4.0f * navButtonWidth + 3.0f * navGap;
    const float sidePadding = std::max(14.0f, width * 0.02f);
    const float zoneGap = std::clamp(width * 0.015f, 8.0f, 14.0f);
    const float modeWidth = std::clamp(width * 0.18f, 96.0f, 160.0f);

    // Right side: mode toggle pinned to the right edge.
    modeButtonPosition = {width - sidePadding - modeWidth, centerY};
    if (modeButton) {
        modeButton->setPosition(modeButtonPosition);
        modeButton->setSize({modeWidth, navButtonHeight});
    }

    // Left side: speed label and shorter speed track.
    speedText.setPosition({sidePadding, centerY + 5.0f});
    const float labelWidth = speedText.getLocalBounds().size.x;
    const float desiredTrackWidth = 170.0f;
    const float minTrackWidth = 90.0f;
    const float trackStartX = sidePadding + labelWidth + 14.0f;

    // Center: nav group ideally centered, but clamped to avoid left/right overlap.
    const float desiredNavX = width * 0.5f - navGroupWidth * 0.5f;
    const float leftMaxX = trackStartX + std::clamp(desiredTrackWidth, minTrackWidth, desiredTrackWidth) + zoneGap;
    const float rightMinX = modeButtonPosition.x - zoneGap - navGroupWidth;
    const float navX = std::clamp(desiredNavX, leftMaxX, std::max(leftMaxX, rightMinX));

    // Recompute track width against final nav position to guarantee separation.
    const float maxTrackEnd = navX - zoneGap;
    const float trackWidth = std::clamp(maxTrackEnd - trackStartX, minTrackWidth, desiredTrackWidth);

    speedTrack.setPosition({trackStartX, barY + barHeight * 0.56f});
    speedTrack.setSize({trackWidth, 6.0f});
    speedFill.setPosition(speedTrack.getPosition());
    syncSpeedVisuals();

    if (firstStepButton) {
        firstStepButton->setSize({navButtonWidth, navButtonHeight});
        firstStepButton->setPosition({navX, centerY});
    }
    if (prevStepButton) {
        prevStepButton->setSize({navButtonWidth, navButtonHeight});
        prevStepButton->setPosition({navX + navButtonWidth + navGap, centerY});
    }
    if (nextStepButton) {
        nextStepButton->setSize({navButtonWidth, navButtonHeight});
        nextStepButton->setPosition({navX + 2.0f * (navButtonWidth + navGap), centerY});
    }
    if (finalStepButton) {
        finalStepButton->setSize({navButtonWidth, navButtonHeight});
        finalStepButton->setPosition({navX + 3.0f * (navButtonWidth + navGap), centerY});
    }
}

void PlaybackControlWidget::render(sf::RenderWindow& window) {
    window.draw(bottomBar);
    window.draw(speedTrack);
    window.draw(speedFill);
    window.draw(speedKnob);
    window.draw(speedText);

    if (modeButton) {
        modeButton->render(window);
    }
    if (firstStepButton) {
        firstStepButton->render(window);
    }
    if (prevStepButton) {
        prevStepButton->render(window);
    }
    if (nextStepButton) {
        nextStepButton->render(window);
    }
    if (finalStepButton) {
        finalStepButton->render(window);
    }
}

void PlaybackControlWidget::rebuildModeButton() {
    const std::string label = autoRunMode ? "Mode: Auto" : "Mode: Manual";
    modeButton = std::make_unique<UI::Button>(
        modeButtonPosition,
        sf::Vector2f(170.0f, 36.0f),
        sf::Color(57, 74, 111),
        label,
        18
    );
    modeButton->setCommand(createPlaybackCommand([this]() {
        autoRunMode = !autoRunMode;
        if (onModeToggled) {
            onModeToggled(autoRunMode);
        }
        rebuildModeButton();
    }));
}

void PlaybackControlWidget::updateSpeedFromMouse(float mouseX) {
    const float trackX = speedTrack.getPosition().x;
    const float trackWidth = speedTrack.getSize().x;
    if (trackWidth <= 0.0f) {
        return;
    }

    const float clampedX = std::clamp(mouseX, trackX, trackX + trackWidth);
    const float ratio = (clampedX - trackX) / trackWidth;
    playbackSpeed.setFromRatio(ratio);

    speedText.setString(playbackSpeed.toLabel());
    syncSpeedVisuals();

    if (onSpeedChanged) {
        onSpeedChanged(playbackSpeed.getValue());
    }
}

void PlaybackControlWidget::syncSpeedVisuals() {
    const float trackX = speedTrack.getPosition().x;
    const float trackY = speedTrack.getPosition().y;
    const float trackWidth = speedTrack.getSize().x;
    const float ratio = playbackSpeed.toRatio();
    const float knobX = trackX + trackWidth * ratio;

    speedKnob.setPosition({knobX, trackY + speedTrack.getSize().y * 0.5f});
    speedFill.setSize({knobX - trackX, speedTrack.getSize().y});
}

} // namespace UI
