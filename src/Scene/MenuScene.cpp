#include "MenuScene.h"
#include "SceneManager.h"
#include <iostream>

MenuScene::MenuScene(SceneManager& sceneManager) : manager(sceneManager) {
}

void MenuScene::processEvents(const sf::Event& event) {
    if (const auto* mouseClick = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseClick->button == sf::Mouse::Button::Left) {
            sf::Vector2f clickPos(static_cast<float>(mouseClick->position.x), 
                                  static_cast<float>(mouseClick->position.y));
            //if (quadTreeButton.getGlobalBounds().contains(clickPos)) {
            //    std::cout << "Transitioning to Quad Tree...\n";
            //}
        }
    }
}

void MenuScene::update(float deltaTime) {
}

void MenuScene::render(sf::RenderWindow& window) {
    sf::Font font;
    if (!font.openFromFile("assets/fonts/Roboto-VariableFont_wdth,wght.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return; 
    }

    sf::Text label(font, "Main Menu", 24);
    label.setFillColor(sf::Color::Red);
    label.setStyle(sf::Text::Bold | sf::Text::Underlined);
    label.setPosition({20.0f, 20.0f});

    window.draw(label);
}