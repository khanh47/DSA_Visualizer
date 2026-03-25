#include "MenuScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "SceneCommand.h"
#include <algorithm>
#include <iostream>

MenuScene::MenuScene(SceneManager& sceneManager) : manager(sceneManager) {
    btnMenu = std::make_unique<UI::ButtonMenu>();

    const float windowWidth = 1920.0f;
    const float windowHeight = 1080.0f;
    const sf::Vector2f buttonSize = {220.0f, 220.0f};
    const float spacing = 400.0f;
    const int buttonCount = 4;
    const float totalRowWidth = (buttonCount - 1) * spacing + buttonSize.x;
    const float centeredX = (windowWidth - totalRowWidth) / 2.0f;
    const float centeredY = (windowHeight - buttonSize.y) / 2.0f;

    btnMenu->setLayoutProperties(
        {centeredX, centeredY}, // start position
        buttonSize, // button size
        spacing, // spacing between button
        true, // is horizontal
        sf::Color(100, 149, 237), // color of the button
        28 // size of text
    );

    btnMenu->addButtonAuto("Linked List", createLinkedListSceneCommand(manager));
    btnMenu->addButtonAuto("Hash Table", createHashTableSceneCommand(manager));
    btnMenu->addButtonAuto("Trie", createTrieSceneCommand(manager));
    btnMenu->addButtonAuto("Kruskal", createKruskalSceneCommand(manager));
}

void MenuScene::processEvents(const sf::Event& event) {
    if (btnMenu) {
        btnMenu->processEvent(event);
    }
}

void MenuScene::update(float deltaTime) {
}

void MenuScene::render(sf::RenderWindow& window) {
    sf::Font &font = ResourceManager::getInstance().getFont("Roboto");
    window.clear(sf::Color(8, 28, 62));

    if (btnMenu) {
        const float width = static_cast<float>(window.getSize().x);
        const float height = static_cast<float>(window.getSize().y);
        const float sidePadding = std::max(20.0f, width * 0.04f);
        const float availableWidth = std::max(0.0f, width - 2.0f * sidePadding);
        const float gap = std::clamp((availableWidth - 4.0f * 160.0f) / 3.0f, 16.0f, 90.0f);
        const float buttonWidth = std::clamp((availableWidth - 3.0f * gap) / 4.0f, 130.0f, 220.0f);
        const float buttonHeight = std::clamp(buttonWidth, 130.0f, 220.0f);
        const float spacing = buttonWidth + gap;
        const float totalRowWidth = buttonWidth + 3.0f * spacing;
        const float startX = (width - totalRowWidth) * 0.5f;
        const float startY = (height - buttonHeight) * 0.55f;

        btnMenu->setLayoutProperties(
            {startX, startY},
            {buttonWidth, buttonHeight},
            spacing,
            true,
            sf::Color(100, 149, 237),
            static_cast<unsigned int>(std::clamp(buttonWidth / 7.0f, 20.0f, 32.0f))
        );
    }

    sf::RectangleShape titleDivider;
    titleDivider.setPosition({0.0f, 75.0f});
    titleDivider.setSize({static_cast<float>(window.getSize().x), 1.0f});
    titleDivider.setFillColor(sf::Color(140, 165, 205));
    window.draw(titleDivider);

    sf::Text label(font, "Data Structures Visualizer", 36);
    label.setFillColor(sf::Color::White);
    const sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin({bounds.position.x + bounds.size.x / 2.0f, bounds.position.y});
    label.setPosition({window.getSize().x / 2.0f, 20.0f});

    window.draw(label);
    if (btnMenu) {
        btnMenu->render(window);
    }
}