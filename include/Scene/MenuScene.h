#pragma once
#include "BaseScene.h"
#include <SFML/Graphics.hpp>

// Forward declaration
class SceneManager;

class MenuScene : public BaseScene {
private:
    SceneManager& manager; // The link back to your state stack

public:
    // Constructor requires the manager!
    MenuScene(SceneManager& sceneManager);
    ~MenuScene() override = default;

    void processEvents(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
};