#include "TrieVisualizer.h"
#include "ResourceManager.h"
#include <sstream>
#include <cmath>
#include <algorithm>

TrieVisualizer::TrieVisualizer() {
    font = &ResourceManager::getInstance().getFont("Roboto");
}

void TrieVisualizer::insertWord(const std::string& word) {
    steps.clear();
    currentStep = 0;
    
    recordStep(word, 0, "Starting insertion of: " + word, "INSERT");
    
    for (size_t i = 1; i <= word.length(); ++i) {
        recordStep(word, i, "Inserting character: " + std::string(1, word[i-1]), "INSERT");
    }
    
    trie.insert(word);
    currentWords.push_back(word);
    recordStep(word, word.length(), "Finished inserting: " + word, "INSERT");
    
    currentStep = steps.size() - 1;
}

void TrieVisualizer::searchWord(const std::string& word) {
    steps.clear();
    currentStep = 0;
    
    recordStep(word, 0, "Searching for: " + word, "SEARCH");
    
    TrieNode* current = trie.getRoot();
    bool found = true;
    for (size_t i = 0; i < word.length(); ++i) {
        unsigned char ch = word[i];
        if (current && current->children[ch]) {
            recordStep(word, i + 1, "Found character: " + std::string(1, ch), "SEARCH");
            current = current->children[ch];
        } else {
            recordStep(word, i, "Character '" + std::string(1, ch) + "' not found.", "SEARCH");
            found = false;
            break;
        }
    }
    
    if (found && current->isEndOfWord) {
        recordStep(word, word.length(), "Word '" + word + "' found!", "SEARCH");
    } else if (found) {
        recordStep(word, word.length(), "Word '" + word + "' not found (Not end of word).", "SEARCH");
    }
    
    currentStep = steps.size() - 1;
}

void TrieVisualizer::removeWord(const std::string& word) {
    steps.clear();
    currentStep = 0;
    
    recordStep(word, 0, "Removing word: " + word, "REMOVE");
    
    trie.remove(word);
    
    auto it = std::find(currentWords.begin(), currentWords.end(), word);
    if (it != currentWords.end()) {
        currentWords.erase(it);
    }
    
    recordStep(word, 0, "Finished removing: " + word, "REMOVE");
    currentStep = steps.size() - 1;
}

void TrieVisualizer::updateVisualization(float windowWidth, float windowHeight) {
    visualNodes.clear();
    edges.clear();
    
    if (steps.empty() || !font) return;

    const TrieStep& state = steps[currentStep];
    
    Trie tempTrie;
    for (const std::string& w : state.words) {
        tempTrie.insert(w);
    }
    if (state.operation == "INSERT" && state.activeCharIndex > 0) {
        tempTrie.insert(state.activeWord.substr(0, state.activeCharIndex));
    }

    TrieNode* rootNode = tempTrie.getRoot();
    if (!rootNode) return;

    std::map<TrieNode*, float> widths;
    calculateSubtreeWidths(rootNode, widths);

    float startX = windowWidth / 2.0f;
    float startY = 100.0f;

    buildTreeLayout(rootNode, startX, startY, "", state, widths, windowWidth, windowHeight);
}

float TrieVisualizer::calculateSubtreeWidths(TrieNode* node, std::map<TrieNode*, float>& widths) {
    if (!node) return 0.0f;
    float w = 0.0f;
    bool hasChildren = false;
    
    for (int i = 0; i < 256; ++i) {
        if (node->children[i]) {
            w += calculateSubtreeWidths(node->children[i], widths);
            hasChildren = true;
        }
    }
    
    if (!hasChildren) {
        w = 60.0f;
    } else {
        w += 20.0f * ((w / 60.0f) - 1.0f);
    }
    
    widths[node] = w;
    return w;
}

void TrieVisualizer::buildTreeLayout(TrieNode* node, float x, float y, const std::string& currentPath, const TrieStep& stepState, std::map<TrieNode*, float>& widths, float windowWidth, float windowHeight) {
    if (!node) return;

    std::string nodeLabel = currentPath.empty() ? "Root" : std::string(1, currentPath.back());
    auto vNode = std::make_unique<UI::VisualNode>(*font, nodeLabel, 20.0f * zoomLevel);

    float centerX = windowWidth / 2.0f;
    float centerY = windowHeight / 2.0f;
    float zoomedX = centerX + (x - centerX) * zoomLevel;
    float zoomedY = centerY + (y - centerY) * zoomLevel;
    sf::Vector2f finalPos(zoomedX + panOffset.x, zoomedY + panOffset.y);
    
    vNode->setPosition(finalPos);

    if (node->isEndOfWord) {
        vNode->setFillColor(sf::Color(100, 255, 100));
    } else {
        vNode->setFillColor(sf::Color::White);
    }

    bool isHighlighted = false;
    if (stepState.activeCharIndex > 0) {
        std::string activePrefix = stepState.activeWord.substr(0, stepState.activeCharIndex);
        if (currentPath == activePrefix) {
            isHighlighted = true;
        }
    } else if (stepState.activeCharIndex == 0 && currentPath.empty()) {
        isHighlighted = true;
    }

    if (isHighlighted) {
        if (stepState.operation == "SEARCH" && static_cast<size_t>(stepState.activeCharIndex) == stepState.activeWord.length()) {
            vNode->setFillColor(sf::Color(255, 200, 0));
        } else if (stepState.operation == "REMOVE") {
            vNode->setFillColor(sf::Color(255, 100, 100));
        } else {
            vNode->setFillColor(sf::Color(255, 165, 0));
        }
    }

    visualNodes.push_back(std::move(vNode));

    float startX = x - widths[node] / 2.0f;
    for (int i = 0; i < 256; ++i) {
        if (node->children[i]) {
            float childW = widths[node->children[i]];
            float childX = startX + childW / 2.0f;
            float childY = y + 80.0f;

            float childZoomedX = centerX + (childX - centerX) * zoomLevel;
            float childZoomedY = centerY + (childY - centerY) * zoomLevel;
            sf::Vector2f childFinalPos(childZoomedX + panOffset.x, childZoomedY + panOffset.y);
            
            edges.push_back({finalPos, childFinalPos});

            buildTreeLayout(node->children[i], childX, childY, currentPath + static_cast<char>(i), stepState, widths, windowWidth, windowHeight);
            
            startX += childW;
        }
    }
}

void TrieVisualizer::recordStep(const std::string& activeWord, int charIndex, const std::string& desc, const std::string& op) {
    TrieStep step;
    step.words = currentWords;
    step.activeWord = activeWord;
    step.activeCharIndex = charIndex;
    step.description = desc;
    step.operation = op;
    steps.push_back(step);
}

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
    oss << "Trie Visualizer\n";
    oss << "Current Step: " << (currentStep + 1) << "/" << steps.size() << "\n";
    if (currentStep < static_cast<int>(steps.size())) {
        oss << "Status: " << steps[currentStep].description;
    }
    return oss.str();
}

void TrieVisualizer::setPlaybackSpeed(float speed) {
    playbackSpeed = std::max(0.1f, speed);
}

void TrieVisualizer::setAutoRun(bool value) {
    autoRun = value;
    if (!autoRun) {
        elapsedTime = 0.0f;
    }
}

void TrieVisualizer::goToFirstStep() {
    currentStep = 0;
    elapsedTime = 0.0f;
    updateVisualization();
}

void TrieVisualizer::goToPreviousStep() {
    if (currentStep > 0) {
        currentStep--;
        elapsedTime = 0.0f;
        updateVisualization();
    }
}

void TrieVisualizer::goToNextStep() {
    if (currentStep < static_cast<int>(steps.size()) - 1) {
        currentStep++;
        elapsedTime = 0.0f;
        updateVisualization();
    }
}

void TrieVisualizer::goToFinalStep() {
    if (!steps.empty()) {
        currentStep = steps.size() - 1;
        elapsedTime = 0.0f;
        updateVisualization();
    }
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

    for (const auto& edge : edges) {
        sf::Vertex line[] = {
            sf::Vertex(edge.first, sf::Color(150, 150, 150)),
            sf::Vertex(edge.second, sf::Color(150, 150, 150))
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }

    for (auto& node : visualNodes) {
        node->render(window);
    }
}
