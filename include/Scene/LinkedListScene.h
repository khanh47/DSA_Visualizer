#pragma once
#include "VisualizationScene.h"
#include <SFML/Graphics.hpp>

class LinkedListScene : public VisualizationScene {
public:
    LinkedListScene(SceneManager& sceneManager);
    virtual ~LinkedListScene() = default;

    // Hook methods from VisualizationScene
    void onInsert(const std::string& value) override;
    void onSearch(const std::string& value) override;
    void onDelete(const std::string& value) override;
    void onUpdate(const std::string& key, const std::string& value) override;
    void onReset() override;
    void onPlaybackSpeedChanged(float speed) override;
    void onTogglePlaybackMode(bool autoRun) override;
    void onGoToFirstStep() override;
    void onGoToPreviousStep() override;
    void onGoToNextStep() override;
    void onGoToFinalStep() override;

protected:
    std::string getSceneTitle() const override;
};
