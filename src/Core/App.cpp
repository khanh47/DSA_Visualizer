#include "App.h"
#include "MenuScene.h"

App::App() : window(sf::VideoMode({1280, 720}), "DSA Visualizer") {
    manager = std::make_unique<SceneManager>();
    window.setFramerateLimit(60);
    manager->pushScene(std::make_unique<MenuScene>(*manager));
}

void App::run() {
    while (window.isOpen()) {
        float deltaTime = dtClock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }
}

void App::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        manager->processEvents(event);
    }
}

void App::update(float deltaTime) {
    manager->update(deltaTime);
}

void App::render() {
    window.clear(sf::Color(240, 240, 240));
    manager->render(window);
    window.display();
}