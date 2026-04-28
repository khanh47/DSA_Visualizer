#include "TrieVisualizer.h"
#include "ResourceManager.h"
#include <sstream>
#include <cmath>
#include <algorithm>

// ── Construction ────────────────────────────────────────────────────────────

TrieVisualizer::TrieVisualizer() {
    font = &ResourceManager::getInstance().getFont("Roboto");
}

// ── Operation entry points ───────────────────────────────────────────────────

void TrieVisualizer::insertWord(const std::string& word) {
    steps.clear();
    currentStep = 0;

    recordStep("", -1, "Starting insertion of: \"" + word + "\"", "INSERT");

    for (size_t i = 1; i <= word.length(); ++i) {
        recordStep(word, static_cast<int>(i),
                   "Inserting character: '" + std::string(1, word[i - 1]) + "'  (prefix: \"" + word.substr(0, i) + "\")",
                   "INSERT");
    }

    trie.insert(word);
    currentWords.push_back(word);

    recordStep(word, static_cast<int>(word.length()),
               "Done! \"" + word + "\" inserted. End-of-word marker set.",
               "INSERT_DONE");

    currentStep = static_cast<int>(steps.size()) - 1;
}

void TrieVisualizer::searchWord(const std::string& word) {
    steps.clear();
    currentStep = 0;

    recordStep("", -1, "Searching for: \"" + word + "\"", "SEARCH");

    TrieNode* current = trie.getRoot();
    bool found = true;
    for (size_t i = 0; i < word.length(); ++i) {
        unsigned char ch = static_cast<unsigned char>(word[i]);
        if (current && current->children[ch]) {
            current = current->children[ch];
            recordStep(word, static_cast<int>(i + 1),
                       "Found '" + std::string(1, word[i]) + "'  (matched prefix: \"" + word.substr(0, i + 1) + "\")",
                       "SEARCH");
        } else {
            recordStep(word, static_cast<int>(i),
                       "Character '" + std::string(1, word[i]) + "' not found — word does not exist.",
                       "SEARCH_FAIL");
            found = false;
            break;
        }
    }

    if (found) {
        if (current && current->isEndOfWord) {
            recordStep(word, static_cast<int>(word.length()),
                       "\"" + word + "\" FOUND! End-of-word marker is set.",
                       "SEARCH_FOUND");
        } else {
            recordStep(word, static_cast<int>(word.length()),
                       "\"" + word + "\" not found — it is a prefix only, not a complete word.",
                       "SEARCH_FAIL");
        }
    }

    currentStep = static_cast<int>(steps.size()) - 1;
}

void TrieVisualizer::removeWord(const std::string& word) {
    steps.clear();
    currentStep = 0;

    recordStep("", -1, "Removing: \"" + word + "\"", "REMOVE");

    // Walk the path highlighting each node before removal
    TrieNode* current = trie.getRoot();
    bool canRemove = true;
    for (size_t i = 0; i < word.length(); ++i) {
        unsigned char ch = static_cast<unsigned char>(word[i]);
        if (current && current->children[ch]) {
            current = current->children[ch];
            recordStep(word, static_cast<int>(i + 1),
                       "Traversing '" + std::string(1, word[i]) + "' to locate word...",
                       "REMOVE");
        } else {
            recordStep(word, static_cast<int>(i),
                       "\"" + word + "\" not found in trie — nothing to remove.",
                       "REMOVE_FAIL");
            canRemove = false;
            break;
        }
    }

    if (canRemove) {
        trie.remove(word);
        auto it = std::find(currentWords.begin(), currentWords.end(), word);
        if (it != currentWords.end()) currentWords.erase(it);

        recordStep("", -1, "Done! \"" + word + "\" removed.", "REMOVE_DONE");
    }

    currentStep = static_cast<int>(steps.size()) - 1;
}

// ── Internal step recording ──────────────────────────────────────────────────

void TrieVisualizer::recordStep(const std::string& activeWord, int charIndex,
                                 const std::string& desc, const std::string& op) {
    TrieStep step;
    step.words           = currentWords;
    step.activeWord      = activeWord;
    step.activeCharIndex = charIndex;
    step.description     = desc;
    step.operation       = op;
    steps.push_back(step);
}

// ── Layout computation ────────────────────────────────────────────────────────

float TrieVisualizer::calculateSubtreeWidths(TrieNode* node, std::map<TrieNode*, float>& widths) {
    if (!node) return 0.0f;
    float w = 0.0f;
    int childCount = 0;

    for (int i = 0; i < 256; ++i) {
        if (node->children[i]) {
            w += calculateSubtreeWidths(node->children[i], widths);
            ++childCount;
        }
    }

    // Minimum leaf width; add small gaps between siblings
    if (childCount == 0) {
        w = 56.0f;
    } else {
        w += 12.0f * (childCount - 1); // gap between children
    }

    widths[node] = w;
    return w;
}

void TrieVisualizer::buildTreeLayout(TrieNode* node, float x, float y,
                                      const std::string& currentPath,
                                      const TrieStep& stepState,
                                      std::map<TrieNode*, float>& widths,
                                      float windowWidth, float windowHeight) {
    if (!node) return;

    // ── Zoom / pan transform ─────────────────────────────────────────────
    float centerX = windowWidth / 2.0f;
    float centerY = windowHeight / 2.0f;
    float zoomedX = centerX + (x - centerX) * zoomLevel;
    float zoomedY = centerY + (y - centerY) * zoomLevel;
    sf::Vector2f finalPos(zoomedX + panOffset.x, zoomedY + panOffset.y);

    // ── Node label ───────────────────────────────────────────────────────
    std::string nodeLabel = currentPath.empty() ? "*" : std::string(1, currentPath.back());
    float nodeRadius = 20.0f * zoomLevel;
    auto vNode = std::make_unique<UI::VisualNode>(*font, nodeLabel, nodeRadius);
    vNode->setPosition(finalPos);

    // ── Determine highlight state ────────────────────────────────────────
    bool isActive = false;
    if (stepState.activeCharIndex >= 0 && !stepState.activeWord.empty()) {
        std::string activePrefix = stepState.activeWord.substr(0, stepState.activeCharIndex);
        isActive = (currentPath == activePrefix);
    } else if (stepState.activeCharIndex == -1 && currentPath.empty()) {
        isActive = true; // highlight root at step 0
    }

    // Default colour
    if (node->isEndOfWord) {
        vNode->setFillColor(sf::Color(100, 220, 100));   // green = end of word
        vNode->setOutlineColor(sf::Color(30, 140, 30));
    } else {
        vNode->setFillColor(sf::Color(230, 235, 255));   // light blue-white
        vNode->setOutlineColor(sf::Color(80, 100, 160));
    }

    // Override with operation-specific highlight
    if (isActive) {
        const std::string& op = stepState.operation;
        if (op == "INSERT") {
            vNode->setFillColor(sf::Color(255, 180, 50));   // amber
            vNode->setOutlineColor(sf::Color(200, 120, 0));
        } else if (op == "INSERT_DONE") {
            vNode->setFillColor(sf::Color(80, 200, 80));    // bright green
            vNode->setOutlineColor(sf::Color(30, 140, 30));
        } else if (op == "SEARCH") {
            vNode->setFillColor(sf::Color(100, 180, 255));  // sky blue
            vNode->setOutlineColor(sf::Color(30, 100, 200));
        } else if (op == "SEARCH_FOUND") {
            vNode->setFillColor(sf::Color(255, 220, 50));   // gold
            vNode->setOutlineColor(sf::Color(200, 160, 0));
        } else if (op == "SEARCH_FAIL") {
            vNode->setFillColor(sf::Color(255, 80, 80));    // red
            vNode->setOutlineColor(sf::Color(180, 0, 0));
        } else if (op == "REMOVE" || op == "REMOVE_DONE") {
            vNode->setFillColor(sf::Color(255, 100, 100));  // red-orange
            vNode->setOutlineColor(sf::Color(180, 30, 30));
        }
    }

    visualNodes.push_back(std::move(vNode));

    // ── Recurse into children ────────────────────────────────────────────
    float childLevelY = y + 75.0f;
    float startX      = x - widths[node] / 2.0f;

    for (int i = 0; i < 256; ++i) {
        if (!node->children[i]) continue;

        float childW = widths[node->children[i]];
        float childX = startX + childW / 2.0f;

        float childZoomedX = centerX + (childX - centerX) * zoomLevel;
        float childZoomedY = centerY + (childLevelY - centerY) * zoomLevel;
        sf::Vector2f childFinalPos(childZoomedX + panOffset.x, childZoomedY + panOffset.y);

        edges.push_back({finalPos, childFinalPos});

        buildTreeLayout(node->children[i], childX, childLevelY,
                        currentPath + static_cast<char>(i),
                        stepState, widths, windowWidth, windowHeight);

        startX += childW + 12.0f;
    }
}

// ── Visualization update ──────────────────────────────────────────────────────

void TrieVisualizer::updateVisualization(float windowWidth, float windowHeight) {
    visualNodes.clear();
    edges.clear();

    if (steps.empty() || !font) return;

    const TrieStep& state = steps[currentStep];

    // Build a temporary trie reflecting the state at this step
    Trie tempTrie;
    for (const std::string& w : state.words)
        tempTrie.insert(w);

    // During INSERT, show partial path being inserted
    if (state.operation == "INSERT" && state.activeCharIndex > 0)
        tempTrie.insert(state.activeWord.substr(0, state.activeCharIndex));

    TrieNode* rootNode = tempTrie.getRoot();
    if (!rootNode) return;

    std::map<TrieNode*, float> widths;
    calculateSubtreeWidths(rootNode, widths);

    // Root starts horizontally centred, with some top padding below the toolbar
    float startX = windowWidth / 2.0f;
    float startY = 195.0f; // below the top bar

    buildTreeLayout(rootNode, startX, startY, "", state, widths, windowWidth, windowHeight);
}

// ── Playback controls ─────────────────────────────────────────────────────────

void TrieVisualizer::setPlaybackSpeed(float speed) { playbackSpeed = std::max(0.1f, speed); }

void TrieVisualizer::setAutoRun(bool value) {
    autoRun = value;
    if (!autoRun) elapsedTime = 0.0f;
}

void TrieVisualizer::goToFirstStep() {
    currentStep = 0;
    elapsedTime = 0.0f;
    updateVisualization();
}

void TrieVisualizer::goToPreviousStep() {
    if (currentStep > 0) { --currentStep; elapsedTime = 0.0f; updateVisualization(); }
}

void TrieVisualizer::goToNextStep() {
    if (currentStep < static_cast<int>(steps.size()) - 1) {
        ++currentStep; elapsedTime = 0.0f; updateVisualization();
    }
}

void TrieVisualizer::goToFinalStep() {
    if (!steps.empty()) {
        currentStep = static_cast<int>(steps.size()) - 1;
        elapsedTime = 0.0f;
        updateVisualization();
    }
}

// ── BaseVisualizer interface ──────────────────────────────────────────────────

void TrieVisualizer::reset() {
    trie = Trie();
    currentWords.clear();
    steps.clear();
    currentStep = 0;
    elapsedTime = 0.0f;
    visualNodes.clear();
    edges.clear();
}

std::string TrieVisualizer::getProperties() const {
    std::ostringstream oss;
    oss << "Trie Visualizer | Step " << (currentStep + 1) << " / " << steps.size();
    if (currentStep < static_cast<int>(steps.size()))
        oss << "\n" << steps[currentStep].description;
    return oss.str();
}

void TrieVisualizer::processEvents(const sf::Event& event) {
    handleZoomPanEvents(event);
}

void TrieVisualizer::update(float deltaTime) {
    if (autoRun && currentStep < static_cast<int>(steps.size()) - 1) {
        elapsedTime += deltaTime * playbackSpeed;
        if (elapsedTime >= 1.0f) {
            goToNextStep();
            elapsedTime = 0.0f;
        }
    }
}

void TrieVisualizer::render(sf::RenderWindow& window) {
    sf::Vector2u windowSize = window.getSize();
    updateVisualization(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));

    // Draw edges first (behind nodes)
    for (const auto& edge : edges) {
        sf::Vertex line[] = {
            sf::Vertex{edge.first,  sf::Color(120, 140, 200)},
            sf::Vertex{edge.second, sf::Color(120, 140, 200)}
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }

    // Draw nodes on top
    for (auto& node : visualNodes)
        node->render(window);
}
