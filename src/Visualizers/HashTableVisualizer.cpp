#include "HashTableVisualizer.h"
#include "ResourceManager.h"
#include <cmath>
#include <algorithm>

HashTableVisualizer::HashTableVisualizer() {
    font = ResourceManager::getInstance().getFont("Roboto");
    setMode(HashMode::CHAINING); 
}

void HashTableVisualizer::setMode(HashMode mode) {
    if (mode == HashMode::CHAINING) {
        dataStructure = std::make_unique<ChainingHashTable>(7);
    } 
    reset();
}

void HashTableVisualizer::reset() {
    if (dataStructure) dataStructure->clear();
    clearHistory();
    recordState(); 
}

void HashTableVisualizer::setPlaybackSpeed(float speed) {
    currentSpeed = speed; 
}

// ---------------------------------------------------
// SNAPSHOT SYSTEM
// ---------------------------------------------------
void HashTableVisualizer::clearHistory() {
    history.clear();
    currentStep = 0;
}

void HashTableVisualizer::recordState(int bucket, int nodeIdx, int codeLine) {
    HashSnapshot snap;
    snap.highlightBucket = bucket;
    snap.highlightNodeIdx = nodeIdx;
    snap.pseudoCodeLine = codeLine;
    
    auto* chainingData = dynamic_cast<ChainingHashTable*>(dataStructure.get());
    if (chainingData) {
        snap.tableState = chainingData->getTable(); 
    }
    history.push_back(snap);
}

void HashTableVisualizer::setStep(int stepIndex) {
    if (stepIndex >= 0 && stepIndex < (int)history.size()) {
        currentStep = stepIndex;
    }
}

// ---------------------------------------------------
// UPDATE & RENDER
// ---------------------------------------------------
void HashTableVisualizer::update(float deltaTime) {
    // Only used to keep the pulsing sine wave moving
    elapsedTime += deltaTime * currentSpeed; 
}

void HashTableVisualizer::render(sf::RenderWindow& window) {
    if (dataStructure && dataStructure->getMode() == HashMode::CHAINING) {
        renderChaining(window);
    }
}

void HashTableVisualizer::renderChaining(sf::RenderWindow& window) {
    if (history.empty()) return;

    // LẤY DỮ LIỆU TỪ BỨC ẢNH CỦA BƯỚC HIỆN TẠI RA ĐỂ VẼ
    const auto& state = history[currentStep];
    const auto& table = state.tableState;

    const float startX = 150.0f, startY = 250.0f;
    const float boxH = 50.0f, vGap = 20.0f, hGap = 60.0f;

    for (int i = 0; i < (int)table.size(); ++i) {
        float currentY = startY + i * (boxH + vGap);

        // 1. VẼ CỘT INDEX (Sẽ sáng đỏ nhấp nháy nếu được target)
        sf::Color bucketColor = sf::Color(50, 66, 96);
        if (i == state.highlightBucket) {
            float pulse = (std::sin(elapsedTime * 8.0f) + 1.0f) / 2.0f;
            std::uint8_t r = static_cast<std::uint8_t>(180 + pulse * 75);
            bucketColor = sf::Color(r, 60, 60);
        }
        
        // Cột index luôn rộng 100px
        drawBox(window, startX, currentY, std::to_string(i), bucketColor);

        float currentX = startX + 100.0f + hGap;
        float prevX = startX + 100.0f;

        // 2. VẼ CÁC NODE TRONG DANH SÁCH (CHAINING)
        int nodeCount = 0;
        for (const auto& node : table[i]) {
            drawArrow(window, prevX, currentY + boxH / 2.0f, currentX, currentY + boxH / 2.0f);
            
            sf::Color nodeColor = sf::Color(100, 149, 237); // Xanh lam gốc
            bool isTargeted = node.isHighlighted || (i == state.highlightBucket && nodeCount == state.highlightNodeIdx);

            if (isTargeted) {
                // Nhấp nháy Đỏ - Vàng
                float pulse = (std::sin(elapsedTime * 8.0f) + 1.0f) / 2.0f;
                std::uint8_t greenChannel = static_cast<std::uint8_t>(50 + pulse * 205);
                nodeColor = sf::Color(255, greenChannel, 50); 
            }
            
            std::string text = node.value.empty() ? node.key : node.key + " : " + node.value;
            
            // Vẽ hộp và lấy lại chiều rộng thực tế của hộp đó (vì chữ có thể rất dài)
            float actualWidth = drawBox(window, currentX, currentY, text, nodeColor);

            prevX = currentX + actualWidth;
            currentX += actualWidth + hGap;
            nodeCount++;
        }
    }
}

// Hàm này bây giờ trả về chiều rộng (float) để vẽ mũi tên tiếp theo cho chuẩn
float HashTableVisualizer::drawBox(sf::RenderWindow& window, float x, float y, const std::string& textStr, sf::Color bgColor) {
    sf::Text text(font, textStr, 22);
    text.setFillColor(sf::Color::White);
    sf::FloatRect bounds = text.getLocalBounds();
    
    // Auto-resize width based on text length
    float boxWidth = std::max(100.0f, bounds.size.x + 30.0f); 
    float boxHeight = 50.0f;
    float cornerRadius = 12.0f; 

    sf::ConvexShape box;
    const int pointsPerCorner = 10; 
    box.setPointCount(pointsPerCorner * 4);
    const float pi = 3.141592654f;
    int pointIndex = 0;

    auto addCorner = [&](float cx, float cy, float startAngle) {
        for (int i = 0; i < pointsPerCorner; ++i) {
            float angle = startAngle + (i * (pi / 2.0f) / (pointsPerCorner - 1));
            box.setPoint(pointIndex++, sf::Vector2f(cx + cornerRadius * std::cos(angle), cy + cornerRadius * std::sin(angle)));
        }
    };

    addCorner(boxWidth - cornerRadius, boxHeight - cornerRadius, 0.0f);            
    addCorner(cornerRadius, boxHeight - cornerRadius, pi / 2.0f);                  
    addCorner(cornerRadius, cornerRadius, pi);                                     
    addCorner(boxWidth - cornerRadius, cornerRadius, 3.0f * pi / 2.0f);            

    box.setPosition({x, y});
    box.setFillColor(bgColor);
    box.setOutlineThickness(2.0f);
    box.setOutlineColor(sf::Color::Black);
    window.draw(box);

    text.setOrigin({bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f});
    text.setPosition({x + boxWidth / 2.0f, y + boxHeight / 2.0f});
    window.draw(text);

    return boxWidth;
}

void HashTableVisualizer::drawArrow(sf::RenderWindow& window, float x1, float y1, float x2, float y2) {
    sf::Vertex line[] = { 
        sf::Vertex{sf::Vector2f(x1, y1), sf::Color::Black}, 
        sf::Vertex{sf::Vector2f(x2, y2), sf::Color::Black} 
    };
    window.draw(line, 2, sf::PrimitiveType::Lines);

    float dx = x2 - x1;
    float dy = y2 - y1;
    float angle = std::atan2(dy, dx); 
    float arrowLength = 12.0f;          
    float arrowAngle = 3.14159f / 6.0f; 

    sf::Vector2f tip(x2, y2);
    sf::Vector2f leftPoint(x2 - arrowLength * std::cos(angle - arrowAngle), y2 - arrowLength * std::sin(angle - arrowAngle));
    sf::Vector2f rightPoint(x2 - arrowLength * std::cos(angle + arrowAngle), y2 - arrowLength * std::sin(angle + arrowAngle));

    sf::Vertex arrowhead[] = {
        sf::Vertex{tip, sf::Color::Black},
        sf::Vertex{leftPoint, sf::Color::Black},
        sf::Vertex{rightPoint, sf::Color::Black}
    };
    window.draw(arrowhead, 3, sf::PrimitiveType::Triangles);
}

int HashTableVisualizer::getCurrentPseudocodeLine() const {
    if (history.empty() || currentStep < 0 || currentStep >= (int)history.size()) {
        return -1; // Trả về -1 để tắt đèn bôi vàng
    }
    return history[currentStep].pseudoCodeLine;
}