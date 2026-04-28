#pragma once
#include "VisualizationScene.h"
#include "HashTableVisualizer.h"
#include "SelectBox.h"
#include <memory>

class HashTableScene : public VisualizationScene {
private:
    HashTableVisualizer* getHashVisualizer();

        // --- CÁC BIẾN RIÊNG CHO HASH TABLE ---
    sf::Text formulaText;             // Chữ hiển thị công thức ở phía trên
    std::vector<std::string> mathSteps; // Danh sách các bước giải toán
    float mathTimer = 0.0f;           // Đồng hồ đếm bước
    int currentStepIndex = -1;        // Chỉ số bước hiện tại


    int pendingAnimIndex = -1; // Lưu tạm Index đợi vẽ
    int pendingOpType = 0; // --- THÊM BIẾN NÀY (0: Không có gì, 1: Insert, 2: Delete) ---
    void onStatusSequenceFinished() override; // Hàm nhận tín hiệu từ lớp cha

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