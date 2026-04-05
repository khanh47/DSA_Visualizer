#include "HashTableVisualizer.h"
#include "ResourceManager.h"
#include <cmath>

HashTableVisualizer::HashTableVisualizer() {
    font = ResourceManager::getInstance().getFont("Roboto");
    setMode(HashMode::CHAINING); // Default mode
}

void HashTableVisualizer::triggerAnimation() {
    animationTimer = 2.0f; // Đặt thời gian nhấp nháy là 2 giây
}

void HashTableVisualizer::setPlaybackSpeed(float speed) {
    currentSpeed = speed; // Gán tốc độ từ thanh trượt vào biến
}

void HashTableVisualizer::setMode(HashMode mode) {
    if (mode == HashMode::CHAINING) {
        dataStructure = std::make_unique<ChainingHashTable>(7);
    } 
    // Add other modes here later:
    // else if (mode == HashMode::LINEAR_PROBING) { ... }
}

void HashTableVisualizer::reset() {
    if (dataStructure) dataStructure->clear();
}

void HashTableVisualizer::render(sf::RenderWindow& window) {
    if (!dataStructure) return;

    if (dataStructure->getMode() == HashMode::CHAINING) {
        renderChaining(window);
    }
    // else if (dataStructure->getMode() == HashMode::LINEAR_PROBING) { renderLinearProbing(window); }
}

void HashTableVisualizer::renderChaining(sf::RenderWindow& window) {
    // Cast safely to ChainingHashTable to access the 2D vector
    auto* chainingData = dynamic_cast<ChainingHashTable*>(dataStructure.get());
    if (!chainingData) return;

    const auto& table = chainingData->getTable();
    int capacity = chainingData->getCapacity();

    const float startX = 150.0f, startY = 250.0f;
    const float boxW = 100.0f, boxH = 50.0f, vGap = 20.0f, hGap = 60.0f;

    for (int i = 0; i < capacity; ++i) {
        float currentY = startY + i * (boxH + vGap);
        drawBox(window, startX, currentY, "Index " + std::to_string(i), sf::Color(50, 66, 96));

        float currentX = startX + boxW + hGap;
        float prevX = startX + boxW;

        for (const auto& node : table[i]) {
                drawArrow(window, prevX, currentY + boxH / 2.0f, currentX, currentY + boxH / 2.0f);
    
                sf::Color nodeColor;

                if (node.isHighlighted) {
                    // --- ANIMATION NHẤP NHÁY Ở ĐÂY ---
                    // Dùng hàm sin() để tạo dao động từ 0.0 đến 1.0 theo thời gian
                    float pulse = (std::sin(elapsedTime * 8.0f) + 1.0f) / 2.0f;
                    
                    // Trộn màu Đỏ (255, 50, 50) và màu Vàng (255, 255, 50) theo nhịp đập
                    std::uint8_t greenChannel = static_cast<std::uint8_t>(50 + pulse * 205);
                    nodeColor = sf::Color(255, greenChannel, 50);
                } else {
                    nodeColor = sf::Color(100, 149, 237); // Màu xanh lam bình thường
                }
                
                std::string text = node.value.empty() ? node.key : node.key + ":" + node.value;
                drawBox(window, currentX, currentY, text, nodeColor);

                prevX = currentX + boxW;
                currentX += boxW + hGap;
        }
    }
}

void HashTableVisualizer::drawBox(sf::RenderWindow& window, float x, float y, const std::string& textStr, sf::Color bgColor) {
    sf::RectangleShape box({100.0f, 50.0f});
    box.setPosition({x, y});
    box.setFillColor(bgColor);
    box.setOutlineThickness(2.0f);
    box.setOutlineColor(sf::Color::Black);
    window.draw(box);

    sf::Text text(font, textStr, 18);
    text.setFillColor(sf::Color::White);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f});
    text.setPosition({x + 50.0f, y + 25.0f});
    window.draw(text);
}

void HashTableVisualizer::drawArrow(sf::RenderWindow& window, float x1, float y1, float x2, float y2) {
    // 1. Draw the main body of the arrow (the line)
    sf::Vertex line[] = { 
        sf::Vertex{sf::Vector2f(x1, y1), sf::Color::Black}, 
        sf::Vertex{sf::Vector2f(x2, y2), sf::Color::Black} 
    };
    window.draw(line, 2, sf::PrimitiveType::Lines);

    // 2. Calculate coordinates for the arrowhead (triangle)
    float dx = x2 - x1;
    float dy = y2 - y1;
    float angle = std::atan2(dy, dx); // Calculate the angle of the line

    // Arrowhead configuration parameters
    float arrowLength = 12.0f;          // Length of the arrowhead sides
    float arrowAngle = 3.14159f / 6.0f; // Opening angle of the arrowhead (30 degrees = Pi/6 radians)

    // Calculate the two back points to form the triangle
    sf::Vector2f tip(x2, y2);
    sf::Vector2f leftPoint(
        x2 - arrowLength * std::cos(angle - arrowAngle),
        y2 - arrowLength * std::sin(angle - arrowAngle)
    );
    sf::Vector2f rightPoint(
        x2 - arrowLength * std::cos(angle + arrowAngle),
        y2 - arrowLength * std::sin(angle + arrowAngle)
    );

    // 3. Draw the arrowhead
    sf::Vertex arrowhead[] = {
        sf::Vertex{tip, sf::Color::Black},
        sf::Vertex{leftPoint, sf::Color::Black},
        sf::Vertex{rightPoint, sf::Color::Black}
    };
    
    // Use sf::PrimitiveType::Triangles to draw a filled solid triangle
    window.draw(arrowhead, 3, sf::PrimitiveType::Triangles);
}

std::string HashTableVisualizer::getProperties() const {
    return "Hash Table Visualizer Properties";
}

void HashTableVisualizer::update(float deltaTime) {
    elapsedTime += deltaTime * currentSpeed; 

    // Nếu đồng hồ đang chạy (> 0)
    if (animationTimer > 0.0f) {
        // Trừ dần thời gian (chạy nhanh/chậm tùy theo thanh trượt speed)
        animationTimer -= deltaTime * currentSpeed; 
        
        // Khi đồng hồ đếm ngược về 0
        if (animationTimer <= 0.0f) {
            animationTimer = 0.0f;
            if (dataStructure) {
                // TẮT TOÀN BỘ NHẤP NHÁY !
                dataStructure->resetHighlights(); 
            }
        }
    }
}
void HashTableVisualizer::processEvents(const sf::Event& event) {}