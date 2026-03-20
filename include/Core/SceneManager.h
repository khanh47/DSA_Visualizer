#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "BaseScene.h"

class SceneManager {
private:
    std::vector<std::unique_ptr<BaseScene>> scenes;

public:
    SceneManager();
    ~SceneManager();

    void pushScene(std::unique_ptr<BaseScene> newScene);
    void popScene(); 
    void processEvents(const sf::Event& event);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
};
