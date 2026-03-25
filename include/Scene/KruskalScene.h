#pragma once
#include "VisualizationScene.h"
#include <SFML/Graphics.hpp>

class KruskalScene : public VisualizationScene {
public:
    KruskalScene(SceneManager& sceneManager);
    virtual ~KruskalScene() = default;

    void onRandom() override;
    void onRun() override;
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