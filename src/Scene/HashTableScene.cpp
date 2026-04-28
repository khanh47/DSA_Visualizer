#include "HashTableScene.h"
#include "ResourceManager.h"
#include <iostream>

HashTableScene::HashTableScene(SceneManager& sceneManager)
    : VisualizationScene(sceneManager) {
    initializeOperationMenu();
    setVisualizer(new HashTableVisualizer());

}

HashTableVisualizer* HashTableScene::getHashVisualizer() {
    return static_cast<HashTableVisualizer*>(visualizer.get());
}

void HashTableScene::processEvents(const sf::Event& event) { VisualizationScene::processEvents(event); }
void HashTableScene::render(sf::RenderWindow& window) { VisualizationScene::render(window); }

void HashTableScene::update(float deltaTime) {
    VisualizationScene::update(deltaTime); 
    
    // ĐỒNG BỘ: Chữ đang hiện ở dòng nào thì ép Visualizer vẽ bức ảnh của bước đó!
    if (currentStatusIndex != -1) {
        getHashVisualizer()->setStep(currentStatusIndex);
    }
}

// ---------------------------------------------------------
// XỬ LÝ NÚT INSERT
// ---------------------------------------------------------
void HashTableScene::onInsert(const std::string& key, const std::string& value) {
    if (key.empty()) return;

    int sum = 0; for (char c : key) sum += static_cast<int>(c);
    int m = getHashVisualizer()->getData()->getCapacity();
    int index = sum % m;

    if (getHashVisualizer()->getData()->search(key)) {
        getHashVisualizer()->getData()->resetHighlights();
        displayStatus("Failed: Key '" + key + "' already exists!");
        return;
    }
    getHashVisualizer()->getData()->resetHighlights();

    // 1. Dọn dẹp máy ảnh
    getHashVisualizer()->clearHistory();
    std::vector<std::string> sequence;

    // 2. Chụp ảnh Bước 1: Trạng thái ban đầu
    getHashVisualizer()->recordState();
    sequence.push_back("Hashing Key: \"" + key + "\"");

    // 3. Chụp ảnh Bước 2: Sáng ô Index
    getHashVisualizer()->recordState(index);
    sequence.push_back("ASCII Sum: " + std::to_string(sum) + " -> Formula: " + std::to_string(sum) + " % " + std::to_string(m));

    // 4. Chụp ảnh Bước 3: Nhét Node vào và sáng Node đó
    getHashVisualizer()->getData()->insert(key, value);
    auto* chainingData = dynamic_cast<ChainingHashTable*>(getHashVisualizer()->getData());
    int nodeDepth = chainingData ? chainingData->getTable()[index].size() - 1 : 0; // Tính vị trí Node vừa chèn
    
    getHashVisualizer()->recordState(index, nodeDepth);
    sequence.push_back("Result: Index " + std::to_string(index) + " -> Inserted!");

    getHashVisualizer()->getData()->resetHighlights();
    getHashVisualizer()->recordState(-1, -1); 
    sequence.push_back(""); 


    displayStatusSequence(sequence);
}

// ---------------------------------------------------------
// XỬ LÝ NÚT DELETE
// ---------------------------------------------------------
void HashTableScene::onDelete(const std::string& key) {
    if (key.empty()) return;
    
    int sum = 0; for (char c : key) sum += static_cast<int>(c);
    int m = getHashVisualizer()->getData()->getCapacity();
    int index = sum % m;

    if (!getHashVisualizer()->getData()->search(key)) {
        getHashVisualizer()->getData()->resetHighlights();
        displayStatus("Delete Failed: Key '" + key + "' not found.");
        return;
    }
    // Ở bước trên hàm search đã tự động bật sáng cái Node cần tìm. Chúng ta giữ nguyên nó!

    getHashVisualizer()->clearHistory();
    std::vector<std::string> sequence;

    // Bước 1: Chụp ảnh hiện tại (Có Node đang sáng sẵn)
    getHashVisualizer()->recordState(index);
    sequence.push_back("Preparing to Delete: \"" + key + "\"");

    // Bước 2: Chụp lại ảnh đó lần nữa để chữ kịp giải thích
    getHashVisualizer()->recordState(index);
    sequence.push_back("Target Index: " + std::to_string(sum) + " % " + std::to_string(m) + " = " + std::to_string(index));

    // Bước 3: Xóa Node và chụp ảnh lại
    getHashVisualizer()->getData()->remove(key);
    getHashVisualizer()->getData()->resetHighlights();
    getHashVisualizer()->recordState(); 
    sequence.push_back("Success: Node removed from the table!");

    getHashVisualizer()->recordState(-1, -1);
    sequence.push_back("");
    
    displayStatusSequence(sequence);
}

// ---------------------------------------------------------
// XỬ LÝ NÚT SEARCH
// ---------------------------------------------------------
void HashTableScene::onSearch(const std::string& key) {
    if (key.empty()) return;
    
    int sum = 0; for (char c : key) sum += static_cast<int>(c);
    int m = getHashVisualizer()->getData()->getCapacity();
    int index = sum % m;

    getHashVisualizer()->clearHistory();
    getHashVisualizer()->getData()->resetHighlights();
    std::vector<std::string> sequence;

    // Bước 1
    getHashVisualizer()->recordState();
    sequence.push_back("Searching for Key: \"" + key + "\"");

    // Bước 2: Bật sáng cột Index
    getHashVisualizer()->recordState(index);
    sequence.push_back("Formula: " + std::to_string(sum) + " % " + std::to_string(m) + " = " + std::to_string(index));

    // Bước 3
    if (getHashVisualizer()->getData()->search(key)) {
        getHashVisualizer()->recordState(index); // Node đã tự sáng do lệnh search()
        sequence.push_back("Found at Index " + std::to_string(index) + "!");
    } else {
        getHashVisualizer()->recordState(index);
        sequence.push_back("Index " + std::to_string(index) + " checked -> Not Found!");
    }

    getHashVisualizer()->getData()->resetHighlights();
    getHashVisualizer()->recordState(-1, -1);
    sequence.push_back("");
    
    displayStatusSequence(sequence);
}

// ---------------------------------------------------------
// XỬ LÝ NÚT UPDATE
// ---------------------------------------------------------
void HashTableScene::onUpdate(const std::string& key, const std::string& value) {
    if (key.empty()) return;

    int sum = 0; for (char c : key) sum += static_cast<int>(c);
    int m = getHashVisualizer()->getData()->getCapacity();
    int index = sum % m;

    if (!getHashVisualizer()->getData()->search(key)) {
        getHashVisualizer()->getData()->resetHighlights();
        displayStatus("Update Failed: Key '" + key + "' not found.");
        return;
    }

    getHashVisualizer()->clearHistory();
    std::vector<std::string> sequence;

    // Bước 1: Node đang sáng sẵn từ lệnh search bên trên
    getHashVisualizer()->recordState(index);
    sequence.push_back("Updating Key: \"" + key + "\"");

    // Bước 2: Giữ nguyên ảnh cho câu chữ
    getHashVisualizer()->recordState(index);
    sequence.push_back("Target Index: " + std::to_string(sum) + " % " + std::to_string(m) + " = " + std::to_string(index));

    // Bước 3: Cập nhật giá trị mới
    getHashVisualizer()->getData()->update(key, value); 
    getHashVisualizer()->getData()->search(key); // Ép nó sáng đỏ lên
    getHashVisualizer()->recordState(index);
    sequence.push_back("Applying new value...");

    getHashVisualizer()->getData()->resetHighlights();
    getHashVisualizer()->recordState(-1, -1);
    sequence.push_back("");
    
    displayStatusSequence(sequence);
}

// ... Các hàm Playback giữ nguyên
void HashTableScene::onReset() { if(visualizer) visualizer->reset(); }
void HashTableScene::onTogglePlaybackMode(bool a) { if(visualizer) visualizer->setAutoRun(a); }
void HashTableScene::onGoToFirstStep() { VisualizationScene::onGoToFirstStep(); }
void HashTableScene::onGoToPreviousStep() { VisualizationScene::onGoToPreviousStep(); }
void HashTableScene::onGoToNextStep() { VisualizationScene::onGoToNextStep(); }
void HashTableScene::onGoToFinalStep() { VisualizationScene::onGoToFinalStep(); }
std::string HashTableScene::getSceneTitle() const { return "Hash Table Visualization"; }