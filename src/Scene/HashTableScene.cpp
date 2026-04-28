#include "HashTableScene.h"
#include "ResourceManager.h"
#include "PseudocodeData.h"
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
    
    // ĐỒNG BỘ: Chữ đang hiện ở dòng nào thì ép Visualizer vẽ bức ảnh của bước đó!
    if (currentStatusIndex != -1) {
        getHashVisualizer()->setStep(currentStatusIndex);
    }

    VisualizationScene::update(deltaTime); 
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

    pseudocodePanel->setPseudocode(Pseudocode::kHashInsertTitle, Pseudocode::kHashInsert);
    pseudocodePanel->show();

    // 1. Dọn dẹp máy ảnh
    getHashVisualizer()->clearHistory();
    std::vector<std::string> sequence;

    // 2. Chụp ảnh Bước 1: Trạng thái ban đầu
    getHashVisualizer()->recordState(-1, -1, 0);
    sequence.push_back("Hashing Key: \"" + key + "\"");

    // 3. Chụp ảnh Bước 2: Sáng ô Index
    getHashVisualizer()->recordState(-1, -1, 1);
    sequence.push_back("ASCII Sum: " + std::to_string(sum) + " -> Formula: " + std::to_string(sum) + " % " + std::to_string(m));

    // 4. Chụp ảnh Bước 3: Nhét Node vào và sáng Node đó
    getHashVisualizer()->getData()->insert(key, value);
    auto* chainingData = dynamic_cast<ChainingHashTable*>(getHashVisualizer()->getData());
    int nodeDepth = chainingData ? chainingData->getTable()[index].size() - 1 : 0; // Tính vị trí Node vừa chèn
    
    getHashVisualizer()->recordState(index, nodeDepth, 3);
    sequence.push_back("Result: Index " + std::to_string(index) + " -> Inserted!");

    getHashVisualizer()->getData()->resetHighlights();
    getHashVisualizer()->recordState(-1, -1, 4);
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

    // Bật bảng Code cho Search
    pseudocodePanel->setPseudocode(Pseudocode::kHashSearchTitle, Pseudocode::kHashSearch);
    pseudocodePanel->show();

    // Bước 1: Tính Index (Sáng dòng 0)
    getHashVisualizer()->recordState(-1, -1, 0);
    sequence.push_back("Searching for Key: \"" + key + "\"");

    // Bước 2: Dò vào cột Index (Sáng dòng 1)
    getHashVisualizer()->recordState(index, -1, 1);
    sequence.push_back("Formula: " + std::to_string(sum) + " % " + std::to_string(m) + " = " + std::to_string(index));

    if (getHashVisualizer()->getData()->search(key)) {
        // Tìm thấy -> Tính độ sâu của Node để Snapshot
        int nodeDepth = 0;
        auto* chainingData = dynamic_cast<ChainingHashTable*>(getHashVisualizer()->getData());
        if (chainingData) {
            for (const auto& node : chainingData->getTable()[index]) {
                if (node.key == key) break;
                nodeDepth++;
            }
        }
        
        // Bước 3A: Tìm thấy (Sáng dòng 2, sáng Node)
        getHashVisualizer()->recordState(index, nodeDepth, 2);
        sequence.push_back("Found at Index " + std::to_string(index) + "!");
        
    } else {
        // Bước 3B: Không tìm thấy (Sáng dòng 3, cột Index vẫn sáng đỏ)
        getHashVisualizer()->recordState(index, -1, 3);
        sequence.push_back("Index " + std::to_string(index) + " checked -> Not Found!");
    }
    
    // Bước 4: KẾT THÚC (Tắt toàn bộ hiệu ứng, tắt bảng Code)
    getHashVisualizer()->getData()->resetHighlights(); // <-- QUAN TRỌNG: Tắt chớp nháy
    getHashVisualizer()->recordState(-1, -1, -1);      // -1 cho codeLine sẽ làm bảng code tàng hình
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
        displayStatus("Delete Failed: Key not found.");
        return;
    }

    getHashVisualizer()->getData()->resetHighlights(); 

    int nodeDepth = 0;
    auto* chainingData = dynamic_cast<ChainingHashTable*>(getHashVisualizer()->getData());
    if (chainingData) {
        for (const auto& node : chainingData->getTable()[index]) {
            if (node.key == key) break;
            nodeDepth++;
        }
    }

    // Bật bảng Code cho Delete
    pseudocodePanel->setPseudocode(Pseudocode::kHashDeleteTitle, Pseudocode::kHashDelete);
    pseudocodePanel->show();

    getHashVisualizer()->clearHistory();
    std::vector<std::string> sequence;

    // Bước 1: Tính Index (Sáng dòng 0)
    getHashVisualizer()->recordState(-1, -1, 0);
    sequence.push_back("Preparing to Delete: \"" + key + "\"");

    // Bước 2: Dò vào cột (Sáng dòng 1)
    getHashVisualizer()->recordState(index, -1, 1); 
    sequence.push_back("Target Index: " + std::to_string(sum) + " % " + std::to_string(m) + " = " + std::to_string(index));

    // Bước 3: Tìm thấy Node (Sáng dòng 2)
    getHashVisualizer()->recordState(index, nodeDepth, 2);
    sequence.push_back("Key found. Preparing to remove...");

    // Bước 4: Đã xóa Node (Sáng dòng 3)
    getHashVisualizer()->getData()->remove(key);
    getHashVisualizer()->getData()->resetHighlights();
    getHashVisualizer()->recordState(-1, -1, 3); 
    sequence.push_back("Success: Node removed from the table.");

    // Bước 5: Kết thúc (Tắt hiệu ứng, tắt code)
    getHashVisualizer()->getData()->resetHighlights(); // <-- QUAN TRỌNG
    getHashVisualizer()->recordState(-1, -1, -1);
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
        displayStatus("Update Failed: Key not found.");
        return;
    }
    
    // Tìm vị trí nodeDepth hiện tại
    int nodeDepth = 0;
    auto* chainingData = dynamic_cast<ChainingHashTable*>(getHashVisualizer()->getData());
    if (chainingData) {
        for (const auto& node : chainingData->getTable()[index]) {
            if (node.key == key) break;
            nodeDepth++;
        }
    }

    // Bật bảng Code cho Update
    pseudocodePanel->setPseudocode(Pseudocode::kHashUpdateTitle, Pseudocode::kHashUpdate);
    pseudocodePanel->show();

    getHashVisualizer()->clearHistory();
    std::vector<std::string> sequence;

    // Bước 1: Tính Index (Sáng dòng 0)
    getHashVisualizer()->getData()->resetHighlights();
    getHashVisualizer()->recordState(-1, -1, 0);
    sequence.push_back("Updating Key: \"" + key + "\"");

    // Bước 2: Dò cột (Sáng dòng 1)
    getHashVisualizer()->recordState(index, -1, 1);
    sequence.push_back("Target Index: " + std::to_string(sum) + " % " + std::to_string(m) + " = " + std::to_string(index));

    // Bước 3: Tìm thấy Node (Sáng dòng 2)
    getHashVisualizer()->recordState(index, nodeDepth, 2);
    sequence.push_back("Node located. Preparing to update...");

    // Bước 4: Đổi giá trị (Sáng dòng 3)
    getHashVisualizer()->getData()->update(key, value); 
    // Hàm update ngầm gọi search, nên cờ isHighlighted của Node tự động = true
    getHashVisualizer()->recordState(index, nodeDepth, 3); 
    sequence.push_back("Value Updated Successfully!");

    // Bước 5: Kết thúc (Tắt toàn bộ hiệu ứng, tắt code)
    getHashVisualizer()->getData()->resetHighlights(); // <-- QUAN TRỌNG: Tắt chớp nháy sau Update!
    getHashVisualizer()->recordState(-1, -1, -1);
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