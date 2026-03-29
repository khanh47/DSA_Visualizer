#include "HashTableVisualizer.h"
#include "ResourceManager.h"

HashTableVisualizer::HashTableVisualizer() {
    font = ResourceManager::getInstance().getFont("Roboto");
    setMode(HashMode::CHAINING); // Default mode
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
            sf::Color nodeColor = node.isHighlighted ? sf::Color(220, 100, 100) : sf::Color(100, 149, 237);
            
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
        sf::Vertex line[] = { 
        sf::Vertex{sf::Vector2f(x1, y1), sf::Color::Black}, 
        sf::Vertex{sf::Vector2f(x2, y2), sf::Color::Black} 
        };
    window.draw(line, 2, sf::PrimitiveType::Lines);
}

std::string HashTableVisualizer::getProperties() const {
    return "Hash Table Visualizer Properties";
}

void HashTableVisualizer::update(float deltaTime) {}
void HashTableVisualizer::processEvents(const sf::Event& event) {}