#pragma once
#include "VisualizationScene.h"
#include "HashTableVisualizer.h"
#include "SelectBox.h"
#include <memory>

class HashTableScene : public VisualizationScene {
private:
    HashTableVisualizer* getHashVisualizer();

        // --- CÁC BIẾN RIÊNG CHO HASH TABLE ---
    std::vector<std::string> mathSteps; // Danh sách các bước giải toán
    float mathTimer = 0.0f;           // Đồng hồ đếm bước
    int currentStepIndex = -1;        // Chỉ số bước hiện tại



public:
    HashTableScene(SceneManager& sceneManager);

    void processEvents(const sf::Event& event) override;
    void render(sf::RenderWindow& window) override;

    void onInsert(const std::string& key, const std::string& value) override;
    void onSearch(const std::string& value) override;
    void onDelete(const std::string& value) override;
    void onUpdate(const std::string& key, const std::string& value) override;
    void onReset() override;
    
    // Playback functions
    void onTogglePlaybackMode(bool autoRun) override;
    void onGoToFirstStep() override;
    void onGoToPreviousStep() override;
    void onGoToNextStep() override;
    void onGoToFinalStep() override;

    std::string getSceneTitle() const override;

    void update(float deltaTime) override; // Đảm bảo đã override update
};