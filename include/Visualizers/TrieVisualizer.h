#pragma once

#include "BaseVisualizer.h"
#include "Trie.h"
#include "VisualNode.h"
#include <memory>
#include <vector>
#include <string>
#include <map>

// ── Per-step data ────────────────────────────────────────────────────────────

struct TrieStep {
    std::vector<std::string> words;   // words already committed to the trie
    std::string activeWord;           // word being operated on
    int activeCharIndex  = -1;        // how many chars of activeWord are active (-1 = none)
    int pseudocodeLine   = -1;        // which pseudocode line to highlight (-1 = none)
    std::string description;
    std::string operation;            // INSERT | INSERT_DONE | SEARCH | SEARCH_FOUND
                                      // SEARCH_FAIL | REMOVE | REMOVE_DONE | REMOVE_FAIL
};

// ── Per-node render data (position + animation state) ────────────────────────

struct TrieNodeRenderInfo {
    sf::Vector2f  targetPos;      // where the node should rest this frame
    sf::Color     fillColor;
    sf::Color     outlineColor;
    std::string   label;
    std::string   path;           // path from root (empty = root)
    bool          isEndOfWord = false;
    bool          isActive    = false;
};

// ── Animated node (lives frame-to-frame) ─────────────────────────────────────

struct AnimatedNode {
    sf::Vector2f  currentPos;     // smoothly interpolated position
    sf::Vector2f  targetPos;
    float         currentRadius = 20.0f;
    float         targetRadius  = 20.0f;
    float         alpha         = 255.0f; // 0–255, for fade-in / fade-out
    float         targetAlpha   = 255.0f;
    sf::Color     fillColor     = sf::Color::White;
    sf::Color     outlineColor  = sf::Color::Black;
    std::string   label;
    std::string   path;
    bool          isActive      = false;
    // pulse oscillator for the highlighted node
    float         pulsePhase    = 0.0f;
};

// ─────────────────────────────────────────────────────────────────────────────

class TrieVisualizer : public BaseVisualizer {
public:
    TrieVisualizer();
    ~TrieVisualizer() override = default;

    // BaseVisualizer interface
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
    int  getCurrentPseudocodeLine() const override;

    // Operations
    void insertWord(const std::string& word);
    void searchWord(const std::string& word);
    void removeWord(const std::string& word);
    bool wordExists(const std::string& word) const;

private:
    // ── Data ──────────────────────────────────────────────────────────────
    Trie                       trie;
    std::vector<std::string>   currentWords;
    std::vector<TrieStep>      steps;
    int                        currentStep   = 0;
    float                      playbackSpeed = 1.0f;
    bool                       autoRun       = true;
    float                      elapsedTime   = 0.0f;
    sf::Font*                  font          = nullptr;

    // ── Animated node pool ─────────────────────────────────────────────────
    // Keyed by node path string so nodes persist across steps for smooth motion
    std::map<std::string, AnimatedNode> nodePool;

    // Edge list (pairs of paths; resolved to positions each frame)
    std::vector<std::pair<std::string, std::string>> edgeList;

    // ── Scan-line animation (highlight travels char by char) ─────────────
    float  scanProgress  = 0.0f;   // 0 → 1 within the current step
    bool   isScanning    = false;

    // ── Active highlight pulse ─────────────────────────────────────────────
    float  globalPulse   = 0.0f;   // accumulates time for sine wave

    // ── Auto-step timer ───────────────────────────────────────────────────
    bool   isAnimating   = false;

    // ── Layout helpers ─────────────────────────────────────────────────────
    void   recordStep(const std::string& activeWord, int charIndex,
                      const std::string& desc, const std::string& op,
                      int pseudocodeLine = -1);

    // Compute layout for the trie reflected by the given step
    void   computeLayout(const TrieStep& state,
                         float windowWidth, float windowHeight,
                         std::map<std::string, TrieNodeRenderInfo>& outNodes,
                         std::vector<std::pair<std::string,std::string>>& outEdges);

    float  calculateSubtreeWidths(TrieNode* node,
                                  std::map<TrieNode*, float>& widths);
    void   buildTreeLayout(TrieNode* node, float x, float y,
                           const std::string& currentPath,
                           const TrieStep& stepState,
                           std::map<TrieNode*, float>& widths,
                           float windowWidth, float windowHeight,
                           std::map<std::string, TrieNodeRenderInfo>& outNodes,
                           std::vector<std::pair<std::string,std::string>>& outEdges);

    // Update nodePool to match a new layout (smooth interpolation target)
    void   syncNodePool(const std::map<std::string, TrieNodeRenderInfo>& layout,
                        float baseRadius);

    // Drawing helpers
    void   drawEdge(sf::RenderWindow& window,
                    const sf::Vector2f& from, const sf::Vector2f& to,
                    float alpha);
    void   drawDescription(sf::RenderWindow& window,
                           const std::string& desc,
                           float windowWidth, float windowHeight);

    static constexpr float kBaseRadius    = 20.0f;
    static constexpr float kLevelSpacing  = 80.0f;
    static constexpr float kSmoothSpeed   = 8.0f;   // position lerp speed
    static constexpr float kAlphaSpeed    = 6.0f;   // fade speed
    static constexpr float kRadiusSpeed   = 8.0f;
};
