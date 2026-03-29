#include "HashTableScene.h"
#include "ResourceManager.h"
#include <iostream>

HashTableScene::HashTableScene(SceneManager& sceneManager)
    : VisualizationScene(sceneManager) {
    initializeOperationMenu();
    setVisualizer(new HashTableVisualizer());

    // Add a SelectBox (Dropdown) to choose modes
    std::vector<std::string> options = {"Chaining", "Linear Probing (Coming Soon)"};
    modeSelectBox = std::make_unique<UI::SelectBox>(sf::Vector2f(1300.0f, 12.0f), sf::Vector2f(250.0f, 40.0f), options, 20);
}

HashTableVisualizer* HashTableScene::getHashVisualizer() {
    return static_cast<HashTableVisualizer*>(visualizer.get());
}

void HashTableScene::processEvents(const sf::Event& event) {
    VisualizationScene::processEvents(event); // Call base class events

    if (modeSelectBox) {

        std::string previousSelected = modeSelectBox->getSelected();

        modeSelectBox->processEvent(event);

        std::string currentSelected = modeSelectBox->getSelected();
        
        if (currentSelected != previousSelected) {
            if (currentSelected == "Chaining") {
                getHashVisualizer()->setMode(HashMode::CHAINING);
                displayStatus("Mode changed to Chaining");
            } 
            // else if (currentSelected == "Linear Probing (Coming Soon)") {
            //    getHashVisualizer()->setMode(HashMode::LINEAR_PROBING);
            //    displayStatus("Mode changed to Linear Probing");
            // }
        }
    }
}

void HashTableScene::render(sf::RenderWindow& window) {
    VisualizationScene::render(window); // Draw visualizer, topbar, bottombar
    
    // Draw the dropdown ON TOP of everything else
    if (modeSelectBox) {
        modeSelectBox->render(window);
    }
}

void HashTableScene::onInsert(const std::string& value) {
    std::cout << "[DEBUG] Da bam nut Insert. Gia tri nhan duoc tu Textbox la: '" << value << "'" << std::endl;
    if (getHashVisualizer()->getData()->insert(value, "")) displayStatus("Inserted: " + value);
    else displayStatus("Failed: Key exists");
}

void HashTableScene::onSearch(const std::string& value) {
    if (getHashVisualizer()->getData()->search(value)) displayStatus("Found: " + value);
    else displayStatus("Not Found: " + value);
}

void HashTableScene::onDelete(const std::string& value) {
    if (getHashVisualizer()->getData()->remove(value)) displayStatus("Deleted: " + value);
    else displayStatus("Failed to delete");
}

void HashTableScene::onUpdate(const std::string& key, const std::string& value) {
    if (getHashVisualizer()->getData()->update(key, value)) displayStatus("Updated: " + key);
    else displayStatus("Key not found");
}

// ... Keep your playback functions exactly the same ...
void HashTableScene::onReset() { if(visualizer) visualizer->reset(); }
void HashTableScene::onPlaybackSpeedChanged(float s) { if(visualizer) visualizer->setPlaybackSpeed(s); }
void HashTableScene::onTogglePlaybackMode(bool a) { if(visualizer) visualizer->setAutoRun(a); }
void HashTableScene::onGoToFirstStep() { if(visualizer) visualizer->goToFirstStep(); }
void HashTableScene::onGoToPreviousStep() { if(visualizer) visualizer->goToPreviousStep(); }
void HashTableScene::onGoToNextStep() { if(visualizer) visualizer->goToNextStep(); }
void HashTableScene::onGoToFinalStep() { if(visualizer) visualizer->goToFinalStep(); }
std::string HashTableScene::getSceneTitle() const { return "Hash Table Visualization"; }