#include "VisualizationScene.h"
#include "ResourceManager.h"
#include "OperationCommand.h"
#include "SceneCommand.h"
#include <iostream>

VisualizationScene::VisualizationScene(SceneManager& sceneManager)
    : manager(sceneManager),
      statusText(ResourceManager::getInstance().getFont("Roboto"), "") {
    
    // Initialize back button
    statusText.setPosition({50.0f, 195.0f});
    statusText.setCharacterSize(28);
    statusText.setFillColor(sf::Color(30, 30, 30));
    statusText.setStyle(sf::Text::Bold);
    backButton = std::unique_ptr<UI::Button>(new UI::Button({16.0f, 20.0f}, {36.0f, 36.0f}, sf::Color(8, 28, 62), "<", 30));
    backButton->setCommand(createPopSceneCommand(manager));

    playbackWidget = std::make_unique<UI::PlaybackControlWidget>();
    playbackWidget->setOnSpeedChanged([this](float speed) {
        onPlaybackSpeedChanged(speed);
    });
    playbackWidget->setOnModeToggled([this](bool autoRun) {
        onTogglePlaybackMode(autoRun);
    });
    playbackWidget->setOnFirstStep([this]() {
        onGoToFirstStep();
    });
    playbackWidget->setOnPreviousStep([this]() {
        onGoToPreviousStep();
    });
    playbackWidget->setOnNextStep([this]() {
        onGoToNextStep();
    });
    playbackWidget->setOnFinalStep([this]() {
        onGoToFinalStep();
    });
}

void VisualizationScene::initializeOperationMenu() {
    if (!operationMenu) {
        operationMenu = std::make_unique<OperationMenu>(*this);
    }
}

void VisualizationScene::setVisualizer(BaseVisualizer* viz) {
    visualizer.reset(viz);
}

void VisualizationScene::displayStatus(const std::string& message) {
    statusText.setString(message);
}

void VisualizationScene::processEvents(const sf::Event& event) {
    if (visualizer) {
        visualizer->processEvents(event);
    }

    if (operationMenu) {
        operationMenu->processEvents(event);
    }

    if (playbackWidget) {
        playbackWidget->processEvent(event);
    }

    // Process back button last because it can pop (destroy) this scene.
    backButton->processEvent(event);
}

void VisualizationScene::update(float deltaTime) {
    if (visualizer) visualizer->update(deltaTime);

    if (currentStatusIndex != -1 && currentStatusIndex < (int)statusQueue.size() - 1) {
        
        bool isAuto = true;
        if (playbackWidget) {
            isAuto = playbackWidget->isAutoRunMode();
        }

                if (isAuto) {
            statusTimer += deltaTime * playbackSpeedScale; 

            if (statusTimer >= 1.5f) { 
                onGoToNextStep(); // Tự động gọi hàm sang bước tiếp theo
            }
        }
/*
        // Tốc độ đếm giờ phụ thuộc vào thanh trượt
        statusTimer += deltaTime * playbackSpeedScale; 

        // Đặt mốc cơ bản là 1.5 giây (khi ở mức 1.0x)
        if (statusTimer >= 1.5f) { 
            statusTimer = 0.0f;
            currentStatusIndex++;
            statusText.setString(statusQueue[currentStatusIndex]);
        }
*/
    }
}

void VisualizationScene::render(sf::RenderWindow& window) {
    const sf::Vector2u windowSize = window.getSize();
    const float windowHeight = static_cast<float>(windowSize.y);
    const float topBarHeight = 150.0f;
    const float bottomBarHeight = std::clamp(windowHeight * 0.07f, 56.0f, 76.0f);
    const float bottomBarY = windowHeight - bottomBarHeight;

    if (playbackWidget) {
        playbackWidget->layout(windowSize);
    }

    if (visualizer) {
        visualizer->setDragRegion(topBarHeight, bottomBarY);
        visualizer->render(window);
    }

    sf::RectangleShape topBar;
    topBar.setPosition({0.0f, 0.0f});
    topBar.setSize({static_cast<float>(windowSize.x), topBarHeight});
    topBar.setFillColor(sf::Color(8, 28, 62));
    window.draw(topBar);

    sf::RectangleShape titleDivider;
    titleDivider.setPosition({0.0f, 75.0f});
    titleDivider.setSize({static_cast<float>(windowSize.x), 1.0f});
    titleDivider.setFillColor(sf::Color(140, 165, 205));
    window.draw(titleDivider);

    backButton->render(window);
    if (operationMenu) {
        operationMenu->render(window);
    }

    if (playbackWidget) {
        playbackWidget->render(window);
    }

    sf::Font &font = ResourceManager::getInstance().getFont("Roboto");
    sf::Text label(font, getSceneTitle(), 36);
    label.setFillColor(sf::Color::White);
    label.setPosition({72.0f, 12.0f});

    window.draw(label);
    window.draw(statusText);
}

void VisualizationScene::onPlaybackSpeedChanged(float speed) {
    // 1. Cập nhật biến tỉ lệ tốc độ (Dùng cho việc hiện chữ)
    this->playbackSpeedScale = speed;

    // 2. In ra Console để kiểm tra chắc chắn giá trị có thay đổi không
    std::cout << "[DEBUG] Toc do hien tai: " << this->playbackSpeedScale << "x" << std::endl;

    // 3. Cập nhật cho Visualizer (Dùng cho thuật toán)
    if (this->visualizer) {
        this->visualizer->setPlaybackSpeed(speed);
    }
}

void VisualizationScene::displayStatusSequence(const std::vector<std::string>& sequence) {
    statusQueue = sequence;
    currentStatusIndex = 0;
    statusTimer = 0.0f;
    
    // Ngay lập tức hiển thị dòng đầu tiên trong danh sách
    if (!statusQueue.empty()) {
        statusText.setString(statusQueue[0]);
    }
}

void VisualizationScene::onGoToNextStep() {
    // Nếu chưa đến câu cuối cùng thì tiến tới 1 bước
    if (currentStatusIndex != -1 && currentStatusIndex < (int)statusQueue.size() - 1) {
        currentStatusIndex++;
        statusText.setString(statusQueue[currentStatusIndex]);
        statusTimer = 0.0f; // Reset đồng hồ
    }
}

void VisualizationScene::onGoToPreviousStep() {
    // Nếu chưa lùi về câu đầu tiên thì lùi lại 1 bước
    if (currentStatusIndex > 0) {
        currentStatusIndex--;
        statusText.setString(statusQueue[currentStatusIndex]);
        statusTimer = 0.0f;
    }
}

void VisualizationScene::onGoToFirstStep() {
    if (!statusQueue.empty()) {
        currentStatusIndex = 0; // Về index 0
        statusText.setString(statusQueue[currentStatusIndex]);
        statusTimer = 0.0f;
    }
}

void VisualizationScene::onGoToFinalStep() {
    if (!statusQueue.empty()) {
        currentStatusIndex = (int)statusQueue.size() - 1; // Về index cuối cùng
        statusText.setString(statusQueue[currentStatusIndex]);
        statusTimer = 0.0f;
    }
}