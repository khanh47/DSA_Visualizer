#include "VisualizationScene.h"
#include "ResourceManager.h"
#include "OperationCommand.h"
#include "SceneCommand.h"

VisualizationScene::VisualizationScene(SceneManager& sceneManager)
    : manager(sceneManager),
      statusText(ResourceManager::getInstance().getFont("Roboto"), "") {
    
    // Initialize back button
    statusText.setPosition({100.0f, 500.0f});
    statusText.setCharacterSize(28);
    statusText.setFillColor(sf::Color(30, 30, 30));
    statusText.setStyle(sf::Text::Bold);
    backButton = std::unique_ptr<UI::Button>(new UI::Button({16.0f, 20.0f}, {36.0f, 36.0f}, sf::Color(18, 40, 78), "<", 30));
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
    if (visualizer) {
        visualizer->update(deltaTime);
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
