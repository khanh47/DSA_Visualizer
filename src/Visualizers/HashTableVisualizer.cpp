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

    // TÔ ĐỎ NẾU CỘT INDEX ĐANG ĐƯỢC CHỈ ĐỊNH
        sf::Color bucketColor = (i == animBucketIndex || (isDeleting && i == deleteBucket)) 
                                ? sf::Color(220, 60, 60) : sf::Color(50, 66, 96);
        drawBox(window, startX, currentY, std::to_string(i), bucketColor);

        float currentX = startX + boxW + hGap;
        float prevX = startX + boxW;

        int nodeIndex = 0;
        for (const auto& node : table[i]) {
            // 2. HIDE NODE FOR INSERT: Only hide if we are currently animating an INSERT path
            // We use targetBucket to ensure we only hide the node at the destination
            if (isPathAnimating && i == targetBucket && nodeIndex == targetDepth) {
                nodeIndex++;
                continue;
            }

            // 3. DRAW ARROW
            drawArrow(window, prevX, currentY + boxH / 2.0f, currentX, currentY + boxH / 2.0f);
            
            sf::Color nodeColor;

            // 4. HIGHLIGHT NODE COLOR
            // Check if this specific node is the one being deleted
            bool isBeingDeleted = (isDeleting && i == deleteBucket && node.key == deleteTargetKey);

            if (node.isHighlighted || isBeingDeleted) {
                // Pulse effect for both Search result and Deletion target
                float pulse = (std::sin(elapsedTime * 8.0f) + 1.0f) / 2.0f;
                std::uint8_t greenChannel = static_cast<std::uint8_t>(50 + pulse * 205);
                nodeColor = sf::Color(255, greenChannel, 50); // Flashing Red-Yellow
            } else {
                nodeColor = sf::Color(100, 149, 237); // Default Blue
            }
            
            // 5. DRAW NODE BOX
            std::string text = node.value.empty() ? node.key : node.key + " : " + node.value;
            drawBox(window, currentX, currentY, text, nodeColor);

            // Update positions for the next node in the chain
            prevX = currentX + boxW;
            currentX += boxW + hGap;
            nodeIndex++; 
        }
    }
}

void HashTableVisualizer::drawBox(sf::RenderWindow& window, float x, float y, const std::string& textStr, sf::Color bgColor) {
    // Set the dimensions and corner radius
    float boxWidth = 100.0f;
    float boxHeight = 50.0f;
    float cornerRadius = 12.0f; // Độ bo góc (bạn có thể tăng giảm tùy ý)

    // 1. Create a ConvexShape to draw the rounded rectangle
    sf::ConvexShape box;
    const int pointsPerCorner = 10; // Number of points to make the curve smooth
    box.setPointCount(pointsPerCorner * 4);

    const float pi = 3.141592654f;
    int pointIndex = 0;

    // Helper lambda to calculate points for each rounded corner
    auto addCorner = [&](float cx, float cy, float startAngle) {
        for (int i = 0; i < pointsPerCorner; ++i) {
            float angle = startAngle + (i * (pi / 2.0f) / (pointsPerCorner - 1));
            float px = cx + cornerRadius * std::cos(angle);
            float py = cy + cornerRadius * std::sin(angle);
            box.setPoint(pointIndex++, sf::Vector2f(px, py));
        }
    };

    // Calculate and add the 4 corners
    addCorner(boxWidth - cornerRadius, boxHeight - cornerRadius, 0.0f);            // Bottom-Right
    addCorner(cornerRadius, boxHeight - cornerRadius, pi / 2.0f);                  // Bottom-Left
    addCorner(cornerRadius, cornerRadius, pi);                                     // Top-Left
    addCorner(boxWidth - cornerRadius, cornerRadius, 3.0f * pi / 2.0f);            // Top-Right

    // Apply position and styles
    box.setPosition({x, y});
    box.setFillColor(bgColor);
    box.setOutlineThickness(2.0f);
    box.setOutlineColor(sf::Color::Black);
    
    // Draw the rounded box
    window.draw(box);

    // 2. Setup and center the text
    sf::Text text(font, textStr, 22);
    text.setFillColor(sf::Color::White);
    
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f});
    text.setPosition({x + boxWidth / 2.0f, y + boxHeight / 2.0f});
    
    // Draw the text
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

    if (isDeleting) {
        deleteTimer -= deltaTime * currentSpeed;
        if (deleteTimer <= 0.0f) {
            // TIME'S UP: Perform the actual deletion now
            if (dataStructure) dataStructure->remove(deleteTargetKey);
            isDeleting = false;
            deleteBucket = -1;
            deleteTargetKey = "";
        }
    }

    if (isPathAnimating) {
        stepTimer += deltaTime * currentSpeed;
        
        // Step timing (0.8s per stage)
        if (stepTimer >= 0.8f) { 
            stepTimer = 0.0f;
            
            if (animBucketIndex == -1) {
                // STAGE 1: Highlight only the Index box
                animBucketIndex = targetBucket; 
            } else {
                // STAGE 2: Animation finished. Show the new node and start flashing.
                isPathAnimating = false;
                animBucketIndex = -1;
                
                // Turn on the flashing light for the newly inserted node
                auto* chainingData = dynamic_cast<ChainingHashTable*>(dataStructure.get());
                if (chainingData) chainingData->highlightNode(targetBucket, targetDepth);
                
                triggerAnimation(); // Start the 2-second countdown
            }
        }
    } 
    else if (animationTimer > 0.0f) {
        animationTimer -= deltaTime * currentSpeed; 
        if (animationTimer <= 0.0f) {
            animationTimer = 0.0f;
            if (dataStructure) dataStructure->resetHighlights(); 
        }
    }
}
void HashTableVisualizer::processEvents(const sf::Event& event) {}

void HashTableVisualizer::animateInsert(int bucketIndex, int opType) {
    isPathAnimating = true;
    targetBucket = bucketIndex;
    
    // Determine the position of the new node (it's already at the end of the list)
    auto* chainingData = dynamic_cast<ChainingHashTable*>(dataStructure.get());
    if (chainingData) {
        const auto& row = chainingData->getTable()[bucketIndex];
        // TÌM VỊ TRÍ (DEPTH) DỰA TRÊN LOẠI THAO TÁC
        if (opType == 1) { // 1 = INSERT (Lấy Node cuối cùng vừa thêm vào)
            targetDepth = (int)chainingData->getTable()[bucketIndex].size() - 1; 
        } else {
            // For Delete/Search/Update: Find the depth of the specific key
            // (Using deleteTargetKey as a reference if available)
            int depth = 0;
            for (const auto& node : row) {
                if (node.key == deleteTargetKey) {
                    targetDepth = depth;
                    break;
                }
                depth++;
            }
        }
    }

    animBucketIndex = -1; // Start with nothing highlighted
    stepTimer = 0.0f;
    
    if (dataStructure) dataStructure->resetHighlights();
}

void HashTableVisualizer::startDeleteAnimation(int bucketIndex, const std::string& key) {
    isDeleting = true;
    deleteBucket = bucketIndex;
    deleteTargetKey = key;
    deleteTimer = 1.5f; // Show the node for 1.5 seconds before removing
    
    if (dataStructure) dataStructure->resetHighlights();
}