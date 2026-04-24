#include "LinkedListVisualizer.h"
#include "ResourceManager.h"
#include "VisualEdge.h"
#include <sstream>
#include <cmath>
#include <algorithm>

// ── Construction & helpers ──────────────────────────────────────────────────

LinkedListVisualizer::LinkedListVisualizer() {
    font = &ResourceManager::getInstance().getFont("Roboto");
}

std::vector<int> LinkedListVisualizer::listToVector() const {
    std::vector<int> result;
    Node* cur = linkedList.getHead();
    while (cur) { result.push_back(cur->value); cur = cur->next; }
    return result;
}

void LinkedListVisualizer::recordStep(int highlightedIndex, const std::string& description,
                                       int successIndex, int blinkIndex, int searchFoundIndex, int updateIndex, int pseudocodeLine) {
    InsertStep step;
    step.nodeValues       = listToVector();
    step.highlightedIndex = highlightedIndex;
    step.successIndex     = successIndex;
    step.blinkIndex       = blinkIndex;
    step.searchFoundIndex = searchFoundIndex;
    step.updateIndex      = updateIndex;
    step.pseudocodeLine   = pseudocodeLine;
    step.description      = description;
    steps.push_back(step);
}

int LinkedListVisualizer::getCurrentPseudocodeLine() const {
    if (currentStep >= 0 && currentStep < static_cast<int>(steps.size()))
        return steps[currentStep].pseudocodeLine;
    return -1;
}

int LinkedListVisualizer::buildSearchSteps(int target) {
    // pseudocodeLine 0 = "cur = head"
    recordStep(-1, std::string("Start: cur = head"),
               -1, -1, -1, -1, 0);
    Node* cur = linkedList.getHead();
    int visualIndex = 0;
    while (cur) {
        // Sub-step 1: while cur != null  (line 1)
        recordStep(visualIndex, "Visit node at index " + std::to_string(visualIndex + 1),
                   -1, -1, -1, -1, 1);

        // Sub-step 2: if cur.value == target  (line 2)
        bool matched = (cur->value == target);
        std::string checkDesc = "Check: " + std::to_string(cur->value)
                                + (matched ? " == " : " != ")
                                + std::to_string(target);
        recordStep(visualIndex, checkDesc,
                   -1, -1, -1, -1, 2);

        if (matched) return visualIndex;

        // Sub-step 3: cur = cur.next  (line 4) — not matched, advance
        recordStep(visualIndex, "Not matched, cur = cur.next",
                   -1, -1, -1, -1, 4);

        cur = cur->next;
        visualIndex++;
    }
    return -1;
}

int LinkedListVisualizer::buildDeleteSteps(int targetIndex) {
    if (targetIndex < 1) {
        recordStep(-1, "Invalid index " + std::to_string(targetIndex));
        return -1;
    }
    Node* cur = linkedList.getHead();
    if (!cur) return -1;

    int count = 0;
    Node* temp = cur;
    while(temp) { count++; temp=temp->next; }
    if (targetIndex > count) {
        recordStep(-1, "Index " + std::to_string(targetIndex) + " out of bounds");
        return -1;
    }

    recordStep(-1, "Check if index == 1", -1, -1, -1, -1, 0);
    if (targetIndex == 1) {
        recordStep(0, "Index is 1, head = head->next", -1, -1, -1, -1, 1);
        return 0; // The visual index of node to delete
    }

    // line 3 = "cur = head"
    recordStep(0, "Start: cur = head", -1, -1, -1, -1, 3);

    int visualIndex = 0;
    for (int i = 1; i <= targetIndex - 2; i++) {
        // line 4: for loop
        recordStep(visualIndex, "for i = " + std::to_string(i), -1, -1, -1, -1, 4);
        
        // line 5: cur = cur->next
        visualIndex++;
        recordStep(visualIndex, "cur = cur->next", -1, -1, -1, -1, 5);
    }

    // line 6: cur->next = cur->next->next
    recordStep(visualIndex, "cur->next = cur->next->next", -1, -1, -1, -1, 6);

    return targetIndex - 1; // return the visual index of the node to delete
}

int LinkedListVisualizer::buildUpdateSteps(int targetIndex) {
    if (targetIndex < 1) {
        recordStep(-1, "Invalid index " + std::to_string(targetIndex));
        return -1;
    }
    Node* cur = linkedList.getHead();
    if (!cur) return -1;

    int count = 0;
    Node* temp = cur;
    while(temp) { count++; temp=temp->next; }
    if (targetIndex > count) {
        recordStep(-1, "Index " + std::to_string(targetIndex) + " out of bounds");
        return -1;
    }

    // line 0 = "cur = head"
    recordStep(0, "Start: cur = head", -1, -1, -1, -1, 0);

    int visualIndex = 0;
    for (int i = 1; i <= targetIndex - 1; i++) {
        // line 1: for loop
        recordStep(visualIndex, "for i = " + std::to_string(i), -1, -1, -1, -1, 1);
        
        // line 2: cur = cur->next
        visualIndex++;
        recordStep(visualIndex, "cur = cur->next", -1, -1, -1, -1, 2);
    }

    return targetIndex - 1; // return the visual index of the node to update
}

LinkedList&       LinkedListVisualizer::list()       { return linkedList; }
const LinkedList& LinkedListVisualizer::list() const { return linkedList; }

// ── Operations ──────────────────────────────────────────────────────────────

void LinkedListVisualizer::insertValue(int value, bool atHead) {
    steps.clear(); currentStep = 0; nodePositions.clear();

    if (atHead) {
        linkedList.insertAtHead(value);
    } else {
        linkedList.insert(value);
    }

    // Find index of the newly inserted node
    int newIndex = 0;
    if (!atHead) {
        Node* cur = linkedList.getHead();
        while (cur && cur->next) { newIndex++; cur = cur->next; }
    }

    // For insert at head: line 2 = "head = node"; for tail: line 6 = "cur.next = node"
    int finalPseudoLine = atHead ? 2 : 6;
    recordStep(-1, "Inserted " + std::to_string(value) + (atHead ? " at head" : " at tail"), newIndex, -1, -1, -1, finalPseudoLine);

    currentStep = 0; elapsedTime = 0.0f; isAnimating = false;
    droppingNodeIndex = newIndex; dropAnimProgress = 0.0f; isDropAnimating = true;
    updateVisualization();
}

void LinkedListVisualizer::deleteByIndex(int targetIndex) {
    steps.clear(); currentStep = 0; nodePositions.clear();

    int foundIndex = buildDeleteSteps(targetIndex);

    if (foundIndex != -1) {
        int pseudoLine = (targetIndex == 1) ? 2 : 6;
        recordStep(-1, "Found index " + std::to_string(targetIndex) + "! Blinking before removal...",
                   -1, foundIndex, -1, -1, pseudoLine);
        linkedList.remove(targetIndex);
        recordStep(-1, "Successfully removed node at index " + std::to_string(targetIndex),
                   -1, -1, -1, -1, pseudoLine);
    }

    currentStep = 0; elapsedTime = 0.0f; blinkElapsed = 0.0f; isAnimating = true;
    updateVisualization();
}

void LinkedListVisualizer::searchValue(int value) {
    steps.clear(); currentStep = 0; nodePositions.clear();
    isSearchBlinking = false; searchBlinkElapsed = 0.0f; searchBlinkDuration = 0.0f;
    isSearchHighlighting = false; searchHighlightProgress = 0.0f;

    int foundIndex = buildSearchSteps(value);

    if (foundIndex != -1) {
        // pseudocodeLine 3 = "return cur"
        recordStep(-1, "Found " + std::to_string(value) + " at index " + std::to_string(foundIndex + 1) + "!",
                   -1, -1, foundIndex, -1, 3);
    } else {
        // pseudocodeLine 5 = "return NOT_FOUND"
        recordStep(-1, "Value " + std::to_string(value) + " not found in list",
                   -1, -1, -1, -1, 5);
    }

    currentStep = 0; elapsedTime = 0.0f; isAnimating = true;
    updateVisualization();
}

void LinkedListVisualizer::updateByIndex(int targetIndex, int newVal) {
    steps.clear(); currentStep = 0; nodePositions.clear();
    isUpdateAnimating = false; updateAnimProgress = 0.0f; updatingNodeIndex = -1;

    int foundIndex = buildUpdateSteps(targetIndex);

    if (foundIndex != -1) {
        linkedList.update(targetIndex, newVal);
        // pseudocodeLine 3 = "cur.value = newVal"
        recordStep(foundIndex, "Successfully updated node at index " + std::to_string(targetIndex) + " to " + std::to_string(newVal),
                   -1, -1, -1, foundIndex, 3);
    }

    currentStep = 0; elapsedTime = 0.0f; isAnimating = true;
    if (!steps.empty() && steps[currentStep].updateIndex >= 0) {
        isUpdateAnimating = true;
        updateAnimProgress = 0.0f;
        updatingNodeIndex = steps[currentStep].updateIndex;
    }
    updateVisualization();
}

// ── Playback controls ───────────────────────────────────────────────────────

void LinkedListVisualizer::reset() {
    linkedList = LinkedList();
    steps.clear(); currentStep = 0; elapsedTime = 0.0f; visualNodes.clear();
}

std::string LinkedListVisualizer::getProperties() const {
    std::ostringstream oss;
    oss << "Linked List Visualizer\n"
        << "Current Step: " << (currentStep + 1) << "/" << steps.size() << "\n";
    if (currentStep < static_cast<int>(steps.size()))
        oss << "Status: " << steps[currentStep].description;
    return oss.str();
}

void LinkedListVisualizer::setPlaybackSpeed(float speed) { playbackSpeed = std::max(0.1f, speed); }
void LinkedListVisualizer::setAutoRun(bool value) { autoRun = value; if (!autoRun) elapsedTime = 0.0f; }

void LinkedListVisualizer::goToFirstStep()    { currentStep = 0; elapsedTime = 0.0f; updateVisualization(); }
void LinkedListVisualizer::goToPreviousStep() { if (currentStep > 0) { currentStep--; elapsedTime = 0.0f; updateVisualization(); } }
void LinkedListVisualizer::goToFinalStep()    { if (!steps.empty()) { currentStep = steps.size() - 1; elapsedTime = 0.0f; updateVisualization(); } }

void LinkedListVisualizer::goToNextStep() {
    if (currentStep >= static_cast<int>(steps.size()) - 1) return;
    currentStep++; elapsedTime = 0.0f;

    // Trigger search blink animation when entering a search-found step
    if (steps[currentStep].searchFoundIndex >= 0 && !isSearchBlinking && !isSearchHighlighting) {
        isSearchBlinking = true;
        searchBlinkElapsed = 0.0f;
        searchBlinkDuration = 0.0f;
    }
    
    // Trigger update value animation
    if (steps[currentStep].updateIndex >= 0) {
        isUpdateAnimating = true;
        updateAnimProgress = 0.0f;
        updatingNodeIndex = steps[currentStep].updateIndex;
    }
    updateVisualization();
}

void LinkedListVisualizer::processEvents(const sf::Event& event) { handleZoomPanEvents(event); }

// ── Update (animations) ─────────────────────────────────────────────────────

void LinkedListVisualizer::update(float deltaTime) {
    blinkElapsed += deltaTime;  // always tick for blink oscillation

    // Drop animation
    if (isDropAnimating) {
        dropAnimProgress += deltaTime * 2.5f;
        if (dropAnimProgress >= 1.0f) { dropAnimProgress = 1.0f; isDropAnimating = false; droppingNodeIndex = -1; }
    }

    // Fade-out animation (delete)
    if (isFadingOut) {
        fadeProgress += deltaTime * 2.0f;
        if (fadeProgress >= 1.0f) {
            fadeProgress = 1.0f; isFadingOut = false;
            reconnectGapIndex = fadeNodeIndex; fadeNodeIndex = -1;
            goToNextStep();
            isReconnecting = true; reconnectProgress = 0.0f;
        }
        return;
    }

    // Reconnect animation (delete)
    if (isReconnecting) {
        reconnectProgress += deltaTime * 2.5f;
        if (reconnectProgress >= 1.0f) { reconnectProgress = 1.0f; isReconnecting = false; reconnectGapIndex = -1; isAnimating = false; }
        return;
    }

    // Search blink phase (Phase 1: green/orange blink)
    if (isSearchBlinking) {
        searchBlinkElapsed += deltaTime;
        searchBlinkDuration += deltaTime;
        if (searchBlinkDuration >= 0.8f) {
            isSearchBlinking = false;
            isSearchHighlighting = true; searchHighlightProgress = 0.0f;
        }
    }

    // Search highlight phase (Phase 2: scale up + color morph)
    if (isSearchHighlighting) {
        searchHighlightProgress += deltaTime * 1.5f;
        if (searchHighlightProgress >= 1.0f) { searchHighlightProgress = 1.0f; isSearchHighlighting = false; }
    }

    // Update value animation (node pops)
    if (isUpdateAnimating) {
        updateAnimProgress += deltaTime * 1.5f;
        if (updateAnimProgress >= 1.0f) {
            updateAnimProgress = 1.0f;
            isUpdateAnimating = false;
        }
    }

    // Step-based auto-advance
    if ((autoRun || isAnimating) && currentStep < static_cast<int>(steps.size()) - 1) {
        bool isBlinkStep       = steps[currentStep].blinkIndex >= 0;
        bool isSearchFoundStep = steps[currentStep].searchFoundIndex >= 0;
        float stepDelay = isBlinkStep ? 1.5f : (isSearchFoundStep ? 2.5f : 0.75f);

        elapsedTime += deltaTime * playbackSpeed;
        if (elapsedTime >= stepDelay) {
            if (isBlinkStep) {
                isFadingOut = true; fadeProgress = 0.0f;
                fadeNodeIndex = steps[currentStep].blinkIndex; elapsedTime = 0.0f;
            } else {
                goToNextStep();
            }
        }
    }

    // Stop animation at last step (non-delete)
    if (isAnimating && !isFadingOut && !isReconnecting
        && currentStep >= static_cast<int>(steps.size()) - 1) {
        isAnimating = false;
    }
}

// ── Node coloring & scaling ─────────────────────────────────────────────────

void LinkedListVisualizer::applyNodeColor(UI::VisualNode& node, int index,
                                           const InsertStep& step, float& renderedRadius) {
    const sf::Color kGreen(50, 205, 50);
    const sf::Color kOrange(255, 165, 0);
    const sf::Color kBlue(135, 206, 235);

    // Delete-blink (green / orange)
    if (index == step.blinkIndex) {
        float blink = std::sin(blinkElapsed * 12.5f);
        node.setFillColor(blink >= 0.0f ? kGreen : kOrange);
        return;
    }

    // Search-found — Phase 1: blink (green / orange)
    if (index == step.searchFoundIndex && isSearchBlinking) {
        float blink = std::sin(searchBlinkElapsed * 12.5f);
        node.setFillColor(blink >= 0.0f ? kGreen : kOrange);
        return;
    }

    // Search-found — Phase 2: scale up + color transition green → orange
    if (index == step.searchFoundIndex && isSearchHighlighting) {
        float t = std::min(searchHighlightProgress, 1.0f);
        float easedT = 1.0f - std::pow(1.0f - t, 2.0f);

        float scaledRadius = (kNodeRadius + 10.0f * easedT) * zoomLevel;
        node.setRadius(scaledRadius);
        renderedRadius = scaledRadius;

        // green (50,205,50) → orange (255,165,0)
        std::uint8_t r = static_cast<std::uint8_t>(50  + (255 - 50)  * easedT);
        std::uint8_t g = static_cast<std::uint8_t>(205 + (165 - 205) * easedT);
        std::uint8_t b = static_cast<std::uint8_t>(50  + (0   - 50)  * easedT);
        node.setFillColor(sf::Color(r, g, b));
        return;
    }

    // Search-found — animation finished: stay enlarged & orange
    if (index == step.searchFoundIndex && !isSearchHighlighting && !isSearchBlinking) {
        float scaledRadius = (kNodeRadius + 10.0f) * zoomLevel;
        node.setRadius(scaledRadius);
        renderedRadius = scaledRadius;
        node.setFillColor(kOrange);
        return;
    }

    // Other highlights
    if (index == step.successIndex)     { node.setFillColor(kGreen);  return; }
    if (index == step.highlightedIndex) { node.setFillColor(kOrange); return; }

    node.setFillColor(kBlue);
}

// ── Arrow drawing ───────────────────────────────────────────────────────────

void LinkedListVisualizer::drawArrow(sf::RenderWindow& window, size_t fromIdx, size_t toIdx) {
    sf::Vector2f start = nodePositions[fromIdx];
    sf::Vector2f end   = nodePositions[toIdx];

    // Fade arrows connected to fading node
    std::uint8_t alpha = 255;
    if (isFadingOut && fadeNodeIndex >= 0) {
        if (static_cast<int>(fromIdx) == fadeNodeIndex || static_cast<int>(toIdx) == fadeNodeIndex) {
            float easedFade = fadeProgress * fadeProgress;
            alpha = static_cast<std::uint8_t>(255 * (1.0f - easedFade));
        }
    }
    sf::Color edgeColor(0, 0, 0, alpha);

    sf::Vector2f direction = end - start;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length <= 0.001f) return;

    sf::Vector2f dir = direction / length;

    // Use each node's actual rendered radius so arrows stay flush with enlarged nodes
    float startR = nodeRenderedRadii[fromIdx] + 2.5f - 1.5f;
    float endR   = nodeRenderedRadii[toIdx]   + 2.5f - 1.5f;

    sf::Vector2f arrowStart = start + dir * startR;
    sf::Vector2f arrowEnd   = end   - dir * endR;
    float arrowLength = length - startR - endR;
    if (arrowLength <= 0.0f) return;

    float scaledThickness = 2.0f  * zoomLevel;
    float headLength      = 15.0f * zoomLevel;
    float headWidth       = 7.5f  * zoomLevel;
    float angle = std::atan2(direction.y, direction.x) * 180.0f / 3.14159f;

    float bodyLength = arrowLength - headLength;
    if (bodyLength > 0.0f) {
        sf::RectangleShape body(sf::Vector2f(bodyLength, scaledThickness));
        body.setOrigin({0.0f, scaledThickness / 2.0f});
        body.setPosition(arrowStart);
        body.setRotation(sf::degrees(angle));
        body.setFillColor(edgeColor);
        window.draw(body);
    }

    sf::ConvexShape head;
    head.setPointCount(3);
    head.setPoint(0, {0.0f, 0.0f});
    head.setPoint(1, {-headLength,  headWidth});
    head.setPoint(2, {-headLength, -headWidth});
    head.setPosition(arrowEnd);
    head.setRotation(sf::degrees(angle));
    head.setFillColor(edgeColor);
    window.draw(head);
}

// ── Visualization build ─────────────────────────────────────────────────────

void LinkedListVisualizer::updateVisualization(float windowWidth, float windowHeight) {
    visualNodes.clear(); nodePositions.clear(); nodeRenderedRadii.clear();
    if (steps.empty() || !font) return;

    const InsertStep& state = steps[currentStep];
    float totalWidth = state.nodeValues.size() * kSpacing;
    float startX = (windowWidth - totalWidth) / 2.0f + kNodeRadius;
    float startY = windowHeight / 2.0f;
    float centerX = windowWidth / 2.0f, centerY = windowHeight / 2.0f;

    for (size_t i = 0; i < state.nodeValues.size(); ++i) {
        auto node = std::make_unique<UI::VisualNode>(*font, std::to_string(state.nodeValues[i]),
                                                      kNodeRadius * zoomLevel);
        float renderedRadius = kNodeRadius * zoomLevel;

        // Position with zoom + pan
        float posX = startX + i * kSpacing;
        float zoomedX = centerX + (posX - centerX) * zoomLevel;
        float zoomedY = centerY + (startY - centerY) * zoomLevel;
        sf::Vector2f pos(zoomedX + panOffset.x, zoomedY + panOffset.y);

        // Drop animation (insert)
        if (isDropAnimating && static_cast<int>(i) == droppingNodeIndex) {
            float e = 1.0f - std::pow(1.0f - dropAnimProgress, 3.0f);
            pos.y += (1.0f - e) * -150.0f * zoomLevel;
        }

        // Reconnect slide (delete)
        if (isReconnecting && reconnectGapIndex >= 0 && static_cast<int>(i) >= reconnectGapIndex) {
            float e = 1.0f - std::pow(1.0f - reconnectProgress, 2.0f);
            pos.x += (1.0f - e) * kSpacing * zoomLevel;
        }

        node->setPosition(pos);
        nodePositions.push_back(pos);

        // Color / scale
        applyNodeColor(*node, static_cast<int>(i), state, renderedRadius);

        // Update animation (scale pulse and color blend)
        if (isUpdateAnimating && static_cast<int>(i) == updatingNodeIndex) {
            float pop = std::sin(updateAnimProgress * 3.14159f); // 0 -> 1 -> 0
            float scale = 1.0f + 0.35f * pop;
            renderedRadius *= scale;
            node->setRadius(renderedRadius);

            sf::Color current(255, 165, 0); // kOrange
            sf::Color popColor(255, 215, 0); // Gold
            std::uint8_t r = static_cast<std::uint8_t>(current.r + (popColor.r - current.r) * pop);
            std::uint8_t g = static_cast<std::uint8_t>(current.g + (popColor.g - current.g) * pop);
            std::uint8_t b = static_cast<std::uint8_t>(current.b + (popColor.b - current.b) * pop);
            node->setFillColor(sf::Color(r, g, b));
        }

        // Fade-out (delete)
        if (isFadingOut && static_cast<int>(i) == fadeNodeIndex) {
            float e = fadeProgress * fadeProgress;
            node->setAlpha(static_cast<std::uint8_t>(255 * (1.0f - e)));
        }

        nodeRenderedRadii.push_back(renderedRadius);
        visualNodes.push_back(std::move(node));
    }
}

// ── Render ──────────────────────────────────────────────────────────────────

void LinkedListVisualizer::render(sf::RenderWindow& window) {
    sf::Vector2u sz = window.getSize();
    updateVisualization(static_cast<float>(sz.x), static_cast<float>(sz.y));

    for (size_t i = 0; i + 1 < nodePositions.size(); ++i)
        drawArrow(window, i, i + 1);

    for (auto& node : visualNodes)
        node->render(window);
}
