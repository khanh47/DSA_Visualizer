#pragma once
#include "VisualizationScene.h"
#include "HashTableVisualizer.h"
#include "SelectBox.h"
#include <memory>

class HashTableScene : public VisualizationScene {
private:
    std::unique_ptr<UI::SelectBox> modeSelectBox;
    HashTableVisualizer* getHashVisualizer();

public:
    HashTableScene(SceneManager& sceneManager);

    void processEvents(const sf::Event& event) override;
    void render(sf::RenderWindow& window) override;

    void onInsert(const std::string& value) override;
    void onSearch(const std::string& value) override;
    void onDelete(const std::string& value) override;
    void onUpdate(const std::string& key, const std::string& value) override;
    void onReset() override;
    
    // Playback functions
    void onPlaybackSpeedChanged(float speed) override;
    void onTogglePlaybackMode(bool autoRun) override;
    void onGoToFirstStep() override;
    void onGoToPreviousStep() override;
    void onGoToNextStep() override;
    void onGoToFinalStep() override;

    std::string getSceneTitle() const override;
};