#include "HashTableScene.h"
#include "ResourceManager.h"
#include <iostream>

HashTableScene::HashTableScene(SceneManager& sceneManager)
    : VisualizationScene(sceneManager),
    formulaText(ResourceManager::getInstance().getFont("Roboto"), "", 26)  {
    initializeOperationMenu();
    setVisualizer(new HashTableVisualizer());

    // Add a SelectBox (Dropdown) to choose modes
    std::vector<std::string> options = {"Chaining", "Linear Probing (Coming Soon)"};
    modeSelectBox = std::make_unique<UI::SelectBox>(sf::Vector2f(1300.0f, 12.0f), sf::Vector2f(250.0f, 40.0f), options, 20);

    formulaText.setFont(ResourceManager::getInstance().getFont("Roboto"));
    formulaText.setCharacterSize(26);
    formulaText.setFillColor(sf::Color::Black); // Màu vàng cho nổi bật
    formulaText.setStyle(sf::Text::Bold);
    formulaText.setPosition({500.0f, 195.0f}); // Vị trí phía trên menu
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
    VisualizationScene::render(window); // Vẽ lớp cha (Visualizer, Menu...)

    // VẼ THÊM DÒNG CHỮ RIÊNG CỦA HASH TABLE
    if (currentStepIndex != -1) {
        window.draw(formulaText);
    }

    if (modeSelectBox) modeSelectBox->render(window);
}
void HashTableScene::update(float deltaTime) {
    VisualizationScene::update(deltaTime); // Gọi logic của lớp cha

    // Xử lý chạy chữ lần lượt
    if (currentStepIndex != -1 && currentStepIndex < (int)mathSteps.size() - 1) {
        mathTimer += deltaTime;
        if (mathTimer >= 0.7f) { // 0.7 giây đổi bước
            mathTimer = 0.0f;
            currentStepIndex++;
            formulaText.setString(mathSteps[currentStepIndex]);
        }
    }
}

std::string HashTableScene::getHashMathString(const std::string& key) {
    if (key.empty()) return "";
    
    int sum = 0;
    for (char c : key) {
        sum += static_cast<int>(c); 
    }
    
    
    int m = getHashVisualizer()->getData()->getCapacity();
    int index = sum % m;
    
    
    return "Hash(\"" + key + "\") = " + std::to_string(sum) + " % " + std::to_string(m) + " = Index " + std::to_string(index);
}

void HashTableScene::onInsert(const std::string& value) {
    if (value.empty()) return;

    int sum = 0;
    for (char c : value) sum += static_cast<int>(c);
    int m = getHashVisualizer()->getData()->getCapacity();
    int index = sum % m;

    // 1. Chạy thuật toán trước để biết thành công hay thất bại
    bool success = getHashVisualizer()->getData()->insert(value, "");

    // 2. Tạo chuỗi thông báo từng bước
    std::vector<std::string> sequence;
    sequence.push_back("Hashing: \"" + value + "\"");
    sequence.push_back("Hashing: \"" + value + "\" -> Sum ASCII = " + std::to_string(sum));
    sequence.push_back("Formula: " + std::to_string(sum) + " % " + std::to_string(m));

    // 3. Tùy theo kết quả mà in ra câu chốt hạ
    if (success) {
        sequence.push_back("Result: Index " + std::to_string(index) + " -> Inserted!");
        getHashVisualizer()->triggerAnimation(); // Kích hoạt nhấp nháy ô vuông
    } else {
        sequence.push_back("Failed: Key '" + value + "' already exists!");
    }

    displayStatusSequence(sequence);
}
void HashTableScene::onSearch(const std::string& value) {
    if (value.empty()) return;
    
    std::string mathInfo = getHashMathString(value);
    
    if (getHashVisualizer()->getData()->search(value)) {
        displayStatus(mathInfo + " -> Found it!");

        getHashVisualizer()->triggerAnimation();
    } else {
        displayStatus(mathInfo + " -> Not Found!");
    }
}

void HashTableScene::onDelete(const std::string& value) {
    if (value.empty()) return;
    
    std::string mathInfo = getHashMathString(value);

    if (getHashVisualizer()->getData()->remove(value)) {
        displayStatus(mathInfo + " -> Deleted!");
    } else {
        displayStatus("Failed to delete: Key not found.");
    }
}

void HashTableScene::onUpdate(const std::string& key, const std::string& value) {
    if (getHashVisualizer()->getData()->update(key, value)) displayStatus("Updated: " + key);
    else displayStatus("Key not found");
}

void HashTableScene::startMathSequence(const std::string& key) {
    mathSteps.clear();
    int sum = 0;
    for (char c : key) sum += static_cast<int>(c);
    int m = getHashVisualizer()->getData()->getCapacity();
    int index = sum % m;

    // Chia nhỏ công thức thành các phần
    mathSteps.push_back("Key: \"" + key + "\"");
    mathSteps.push_back("Key: \"" + key + "\" -> ASCII Sum: " + std::to_string(sum));
    mathSteps.push_back("Calculation: " + std::to_string(sum) + " % " + std::to_string(m));
    mathSteps.push_back("Target: Index " + std::to_string(index));

    currentStepIndex = 0;
    mathTimer = 0.0f;
    formulaText.setString(mathSteps[0]);
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