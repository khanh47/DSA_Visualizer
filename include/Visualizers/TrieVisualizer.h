#pragma once

#include "BaseVisualizer.h"
#include "Trie.h"
#include "VisualNode.h"
#include <memory>
#include <vector>
#include <string>
#include <map>

struct TrieStep {
    std::vector<std::string> words;
    std::string activeWord;
    int activeCharIndex;
    std::string description;
    std::string operation;
};

class TrieVisualizer : public BaseVisualizer {
public:
    TrieVisualizer();
    ~TrieVisualizer() override = default;

    void processEvents(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void reset() override;
    std::string getProperties() const override;

    void setPlaybackSpeed(float speed) override;
    void setAutoRun(bool autoRun) override;
    void goToFirstStep() override;
    void goToPreviousStep() override;
    void goToNextStep() override;
    void goToFinalStep() override;

    void insertWord(const std::string& word);
    void searchWord(const std::string& word);
    void removeWord(const std::string& word);

private:
    Trie trie;
    std::vector<std::string> currentWords;
    std::vector<TrieStep> steps;
    int currentStep = 0;
    float playbackSpeed = 1.0f;
    bool autoRun = false;
    float elapsedTime = 0.0f;

    std::vector<std::unique_ptr<UI::VisualNode>> visualNodes;
    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> edges;
    sf::Font* font = nullptr;

    void updateVisualization(float windowWidth = 800.0f, float windowHeight = 600.0f);
    void recordStep(const std::string& activeWord, int charIndex, const std::string& desc, const std::string& op);
    
    float calculateSubtreeWidths(TrieNode* node, std::map<TrieNode*, float>& widths);
    void buildTreeLayout(TrieNode* node, float x, float y, const std::string& currentPath, const TrieStep& stepState, std::map<TrieNode*, float>& widths, float windowWidth, float windowHeight);
};
