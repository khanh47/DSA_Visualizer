#include "HashTableScene.h"
#include "ResourceManager.h"
#include <iostream>

HashTableScene::HashTableScene(SceneManager& sceneManager)
    : VisualizationScene(sceneManager),
    formulaText(ResourceManager::getInstance().getFont("Roboto"), "", 26)  {
    initializeOperationMenu();
    setVisualizer(new HashTableVisualizer());

}

HashTableVisualizer* HashTableScene::getHashVisualizer() {
    return static_cast<HashTableVisualizer*>(visualizer.get());
}

void HashTableScene::processEvents(const sf::Event& event) {
    VisualizationScene::processEvents(event); // Call base class events
}

void HashTableScene::render(sf::RenderWindow& window) {
    VisualizationScene::render(window); // Vẽ lớp cha (Visualizer, Menu...)


}
void HashTableScene::update(float deltaTime) {
    VisualizationScene::update(deltaTime); // Gọi logic của lớp cha
}


// ---------------------------------------------------------
// XỬ LÝ NÚT INSERT
// ---------------------------------------------------------
void HashTableScene::onInsert(const std::string& key, const std::string& value) {
    if (key.empty()) return;

    // 1. TÍNH TỔNG ASCII NHƯ THUẬT TOÁN
    int sum = 0;
    for (char c : key) sum += static_cast<int>(c);
    
    int m = getHashVisualizer()->getData()->getCapacity();
    int index = sum % m;

    if (getHashVisualizer()->getData()->search(key)) {
        getHashVisualizer()->getData()->resetHighlights();
        displayStatus("Failed: Key '" + key + "' already exists!");
        return;
    }
    getHashVisualizer()->getData()->resetHighlights();

    pendingAnimIndex = index;
    pendingOpType = 1; // 1 = INSERT

    std::vector<std::string> sequence;
    sequence.push_back("Hashing Key: \"" + key + "\"");
    sequence.push_back("ASCII Sum: " + std::to_string(sum));
    sequence.push_back("Formula: " + std::to_string(sum) + " % " + std::to_string(m));
    sequence.push_back("Result: Index " + std::to_string(index));
    sequence.push_back(""); // Để chữ biến mất
    displayStatusSequence(sequence);
}

// ---------------------------------------------------------
// XỬ LÝ NÚT DELETE
// ---------------------------------------------------------
void HashTableScene::onDelete(const std::string& key) {
    if (key.empty()) return;
    
    int sum = 0;
    for (char c : key) sum += static_cast<int>(c);
    int m = getHashVisualizer()->getData()->getCapacity();
    int index = sum % m;

    if (!getHashVisualizer()->getData()->search(key)) {
        getHashVisualizer()->getData()->resetHighlights();
        displayStatus("Delete Failed: Key '" + key + "' not found.");
        return;
    }
    getHashVisualizer()->getData()->resetHighlights();

    pendingAnimIndex = index;
    pendingOpType = 2; // 2 = DELETE

    std::vector<std::string> sequence;
    sequence.push_back("Preparing to Delete: \"" + key + "\"");
    sequence.push_back("Target Index: " + std::to_string(sum) + " % " + std::to_string(m) + " = " + std::to_string(index));
    sequence.push_back("Index " + std::to_string(index) + " located. Removing...");
    sequence.push_back(""); 
    displayStatusSequence(sequence);
}

// ---------------------------------------------------------
// XỬ LÝ NÚT SEARCH
// ---------------------------------------------------------
void HashTableScene::onSearch(const std::string& key) {
    if (key.empty()) return;
    
    int sum = 0;
    for (char c : key) sum += static_cast<int>(c);
    int m = getHashVisualizer()->getData()->getCapacity();
    int index = sum % m;

    std::vector<std::string> sequence;
    sequence.push_back("Searching for Key: \"" + key + "\"");
    sequence.push_back("ASCII Sum: " + std::to_string(sum) + " -> Formula: " + std::to_string(sum) + " % " + std::to_string(m));

    if (getHashVisualizer()->getData()->search(key)) {
        sequence.push_back("Found at Index " + std::to_string(index) + "!");
        pendingAnimIndex = index;
        pendingOpType = 3; // 3 = SEARCH SUCCESS
    } else {
        sequence.push_back("Index " + std::to_string(index) + " checked -> Not Found!");
        pendingOpType = 0;
    }
    
    getHashVisualizer()->getData()->resetHighlights();
    sequence.push_back(""); 
    displayStatusSequence(sequence);
}

// ---------------------------------------------------------
// XỬ LÝ NÚT UPDATE
// ---------------------------------------------------------
void HashTableScene::onUpdate(const std::string& key, const std::string& value) {
    if (key.empty()) return;

    int sum = 0;
    for (char c : key) sum += static_cast<int>(c);
    int m = getHashVisualizer()->getData()->getCapacity();
    int index = sum % m;

    if (!getHashVisualizer()->getData()->search(key)) {
        getHashVisualizer()->getData()->resetHighlights();
        displayStatus("Update Failed: Key '" + key + "' not found.");
        return;
    }
    getHashVisualizer()->getData()->resetHighlights();

    pendingAnimIndex = index;
    pendingOpType = 4; // 4 = UPDATE

    std::vector<std::string> sequence;
    sequence.push_back("Updating Key: \"" + key + "\"");
    sequence.push_back("Target Index: " + std::to_string(sum) + " % " + std::to_string(m) + " = " + std::to_string(index));
    sequence.push_back("Applying new value...");
    sequence.push_back(""); 
    displayStatusSequence(sequence);
}

// ---------------------------------------------------------
// CALLBACK KHI CHỮ CHẠY XONG -> BẮT ĐẦU VẼ HÌNH
// ---------------------------------------------------------
void HashTableScene::onStatusSequenceFinished() {
    if (pendingAnimIndex != -1) {
        
        if (pendingOpType == 1) { // INSERT
            std::string key = operationMenu->getInputValue(0);
            std::string value = operationMenu->getInputValue(1);
            getHashVisualizer()->getData()->insert(key, value);
            getHashVisualizer()->animateInsert(pendingAnimIndex,1);
        } 
        else if (pendingOpType == 2) { // DELETE
            std::string key = operationMenu->getInputValue(2); // Ô Delete là số 2
            // Start the specific Delete Animation (Flash then remove)
            // This is what makes the node eventually disappear!
            getHashVisualizer()->startDeleteAnimation(pendingAnimIndex, key); 
        }
        else if (pendingOpType == 3) { // SEARCH (Tìm thấy)
            std::string key = operationMenu->getInputValue(3); // Ô Search là số 3
            getHashVisualizer()->getData()->search(key); // Lệnh này làm node.isHighlighted = true
            getHashVisualizer()->triggerAnimation();
        }
        else if (pendingOpType == 4) { // UPDATE
            std::string key = operationMenu->getInputValue(4);   // Ô Key là số 4
            std::string value = operationMenu->getInputValue(5); // Ô Value là số 5
            getHashVisualizer()->getData()->update(key, value);  // DataStructure tự đổi isHighlighted = true
            getHashVisualizer()->triggerAnimation();
        }

        // Reset lại trạng thái chờ
        pendingAnimIndex = -1;
        pendingOpType = 0;
    }
}


// ... Keep your playback functions exactly the same ...
void HashTableScene::onReset() { if(visualizer) visualizer->reset(); }
void HashTableScene::onTogglePlaybackMode(bool a) { if(visualizer) visualizer->setAutoRun(a); }
void HashTableScene::onGoToFirstStep() { 
    VisualizationScene::onGoToFirstStep(); // <--- Mở cửa cho lớp cha lùi chữ
    if(visualizer) visualizer->goToFirstStep(); 
}

void HashTableScene::onGoToPreviousStep() { 
    VisualizationScene::onGoToPreviousStep(); // <--- Mở cửa cho lớp cha lùi chữ
    if(visualizer) visualizer->goToPreviousStep(); 
}

void HashTableScene::onGoToNextStep() { 
    VisualizationScene::onGoToNextStep(); // <--- Mở cửa cho lớp cha tiến chữ
    if(visualizer) visualizer->goToNextStep(); 
}

void HashTableScene::onGoToFinalStep() { 
    VisualizationScene::onGoToFinalStep(); // <--- Mở cửa cho lớp cha tiến chữ
    if(visualizer) visualizer->goToFinalStep(); 
}
std::string HashTableScene::getSceneTitle() const { return "Hash Table Visualization"; }
