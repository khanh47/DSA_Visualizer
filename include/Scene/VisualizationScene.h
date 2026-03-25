#pragma once
#include "BaseScene.h"
#include "Button.h"
#include "BaseVisualizer.h"
#include "SceneManager.h"
#include "OperationMenu.h"
#include "PlaybackControlWidget.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class VisualizationScene : public BaseScene {
public:
    VisualizationScene(SceneManager& manager);
    virtual ~VisualizationScene() = default;

    // Public methods for operation dispatch
    void setVisualizer(BaseVisualizer* visualizer);
    void displayStatus(const std::string& message);

    // BaseScene overrides
    void processEvents(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    // Generic hooks with default no-op behavior.
    virtual void onInsert(const std::string& value) { (void)value; }
    virtual void onSearch(const std::string& value) { (void)value; }
    virtual void onDelete(const std::string& value) { (void)value; }
    virtual void onUpdate(const std::string& key, const std::string& value) {
        (void)key;
        (void)value;
    }

    // Kruskal-specific hooks with default no-op behavior.
    virtual void onRandom() {};
    virtual void onRun() {};

    virtual void onReset() {};

    // Playback hooks for the bottom control bar.
    virtual void onPlaybackSpeedChanged(float speed) { (void)speed; }
    virtual void onTogglePlaybackMode(bool autoRun) { (void)autoRun; }
    virtual void onGoToFirstStep() {}
    virtual void onGoToPreviousStep() {}
    virtual void onGoToNextStep() {}
    virtual void onGoToFinalStep() {}

    // Helper method for child classes to override
    virtual std::string getSceneTitle() const = 0;

protected:
    void initializeOperationMenu();

    SceneManager& manager;
    sf::Text statusText;
    std::unique_ptr<UI::Button> backButton;
    std::unique_ptr<OperationMenu> operationMenu;
    std::unique_ptr<BaseVisualizer> visualizer;
    std::unique_ptr<UI::PlaybackControlWidget> playbackWidget;
};