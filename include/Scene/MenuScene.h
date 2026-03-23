#pragma once
#include "BaseScene.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include "ButtonMenu.h"
#include "ICommand.h"
#include "SceneCommand.h"

// Forward declaration
class SceneManager;

class MenuScene : public BaseScene {
private:
    SceneManager& manager;
    std::unique_ptr<UI::ButtonMenu> btnMenu;

public:
    MenuScene(SceneManager& sceneManager);
    ~MenuScene() override = default;

    void processEvents(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
};