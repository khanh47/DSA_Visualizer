#pragma once
#include <memory>
#include "BaseScene.h"
#include "Button.h"
#include <SFML/Graphics.hpp>
#include "SceneManager.h"

class GraphScene : public BaseScene {
protected:
    SceneManager& manager;
    std::unique_ptr<UI::Button> backButton;

public:
    GraphScene(SceneManager& sceneManager);
    virtual ~GraphScene() = default;

    void processEvents(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
};
