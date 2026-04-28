#include "TrieVisualizer.h"
#include "ResourceManager.h"
#include <sstream>
#include <cmath>
#include <algorithm>

// ── Colour palette ────────────────────────────────────────────────────────────
static const sf::Color kColNormal    (220, 228, 255);
static const sf::Color kColEndOfWord (100, 220, 110);
static const sf::Color kColInsert    (255, 185,  55);
static const sf::Color kColInsertDone( 70, 210,  90);
static const sf::Color kColSearch    (100, 185, 255);
static const sf::Color kColFound     (255, 220,  50);
static const sf::Color kColFail      (255,  80,  80);
static const sf::Color kColRemove    (255, 130,  80);

static const sf::Color kOutNormal    ( 80, 100, 170);
static const sf::Color kOutEndOfWord ( 30, 140,  40);
static const sf::Color kOutActive    (200, 120,   0);
static const sf::Color kOutSearch    ( 30, 100, 200);
static const sf::Color kOutFound     (180, 150,   0);
static const sf::Color kOutFail      (160,   0,   0);
static const sf::Color kOutRemove    (180,  60,  20);

static const sf::Color kEdgeNormal   (130, 150, 210);
static const sf::Color kEdgeActive   (255, 185,  55);

// ── Lerp helpers ──────────────────────────────────────────────────────────────
static sf::Vector2f lerpVec(sf::Vector2f a, sf::Vector2f b, float t) {
    return { a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t };
}
static float clamp01(float v) { return v < 0.f ? 0.f : (v > 1.f ? 1.f : v); }

// ── Constructor ───────────────────────────────────────────────────────────────
TrieVisualizer::TrieVisualizer() {
    font = &ResourceManager::getInstance().getFont("Roboto");
}

// ── Step recording ────────────────────────────────────────────────────────────
void TrieVisualizer::recordStep(const std::string& activeWord, int charIndex,
                                 const std::string& desc, const std::string& op,
                                 int pseudocodeLine) {
    TrieStep step;
    step.words           = currentWords;
    step.activeWord      = activeWord;
    step.activeCharIndex = charIndex;
    step.description     = desc;
    step.operation       = op;
    step.pseudocodeLine  = pseudocodeLine;
    steps.push_back(step);
}

int TrieVisualizer::getCurrentPseudocodeLine() const {
    if (currentStep >= 0 && currentStep < static_cast<int>(steps.size()))
        return steps[currentStep].pseudocodeLine;
    return -1;
}

// ── Operations ────────────────────────────────────────────────────────────────
void TrieVisualizer::insertWord(const std::string& word) {
    steps.clear();
    currentStep = 0;
    isAnimating = true;
    elapsedTime = 0.f;

    // line 0: cur = root
    recordStep("", -1, "Insert \"" + word + "\": cur = root", "INSERT", 0);

    // Simulate traversal to know which nodes already exist
    TrieNode* probe = trie.getRoot();
    for (size_t i = 1; i <= word.length(); ++i) {
        unsigned char ch = static_cast<unsigned char>(word[i-1]);
        bool childMissing = !(probe && probe->children[ch]);

        // line 1: for each char c in word
        recordStep(word, static_cast<int>(i) - 1,
                   "Insert \"" + word + "\": check children['" + word[i-1] + "']",
                   "INSERT", 1);

        if (childMissing) {
            // line 2: if cur.children[c] == null  →  line 3: create new node
            recordStep(word, static_cast<int>(i) - 1,
                       "Insert \"" + word + "\": children['" + word[i-1] + "'] is null - create new node",
                       "INSERT", 2);
            recordStep(word, static_cast<int>(i) - 1,
                       "Insert \"" + word + "\": allocating new node for '" + word[i-1] + "'",
                       "INSERT", 3);
        }

        // line 4: cur = cur.children[c]
        recordStep(word, static_cast<int>(i),
                   "Insert \"" + word + "\": follow '" + word[i-1]
                   + "' (prefix \"" + word.substr(0,i) + "\")",
                   "INSERT", 4);

        if (probe && probe->children[ch])
            probe = probe->children[ch];
    }

    trie.insert(word);
    currentWords.push_back(word);
    // line 5: cur.isEndOfWord = true
    recordStep(word, static_cast<int>(word.length()),
               "Done - \"" + word + "\" inserted. End-of-word node marked green.",
               "INSERT_DONE", 5);
    currentStep = 0;
}

void TrieVisualizer::searchWord(const std::string& word) {
    steps.clear();
    currentStep = 0;
    isAnimating = true;
    elapsedTime = 0.f;

    // line 0: cur = root
    recordStep("", -1, "Search \"" + word + "\": cur = root", "SEARCH", 0);
    TrieNode* cur = trie.getRoot();
    bool found = true;
    for (size_t i = 0; i < word.length(); ++i) {
        unsigned char ch = static_cast<unsigned char>(word[i]);
        // line 1: for each char, line 2: check children
        recordStep(word, static_cast<int>(i),
                   "Search \"" + word + "\": check children['" + word[i] + "']",
                   "SEARCH", 2);
        if (cur && cur->children[ch]) {
            cur = cur->children[ch];
            // line 4: cur = children[c]
            recordStep(word, static_cast<int>(i+1),
                       "Search \"" + word + "\": found '" + word[i]
                       + "' - move to next node", "SEARCH", 4);
        } else {
            // line 3: return NOT_FOUND
            recordStep(word, static_cast<int>(i),
                       "Search \"" + word + "\": '" + word[i] + "' missing - NOT FOUND",
                       "SEARCH_FAIL", 3);
            found = false; break;
        }
    }
    if (found) {
        if (cur && cur->isEndOfWord)
            // line 5: return cur.isEndOfWord (true)
            recordStep(word, static_cast<int>(word.length()),
                       "\"" + word + "\" FOUND! End-of-word marker is set.",
                       "SEARCH_FOUND", 5);
        else
            // line 5: return cur.isEndOfWord (false)
            recordStep(word, static_cast<int>(word.length()),
                       "\"" + word + "\" NOT FOUND - prefix only, not a complete word.",
                       "SEARCH_FAIL", 5);
    }
    currentStep = 0;
}

void TrieVisualizer::removeWord(const std::string& word) {
    steps.clear();
    currentStep = 0;
    isAnimating = true;
    elapsedTime = 0.f;

    // line 0: cur = root
    recordStep("", -1, "Remove \"" + word + "\": cur = root", "REMOVE", 0);
    TrieNode* cur = trie.getRoot();
    bool ok = true;
    for (size_t i = 0; i < word.length(); ++i) {
        unsigned char ch = static_cast<unsigned char>(word[i]);
        // line 2: check children
        recordStep(word, static_cast<int>(i),
                   "Remove \"" + word + "\": check children['" + word[i] + "']",
                   "REMOVE", 2);
        if (cur && cur->children[ch]) {
            cur = cur->children[ch];
            // line 4: cur = children[c]
            recordStep(word, static_cast<int>(i+1),
                       "Remove \"" + word + "\": traversing '" + word[i]
                       + "' (prefix \"" + word.substr(0,i+1) + "\")",
                       "REMOVE", 4);
        } else {
            // line 3: return (word not found)
            recordStep(word, static_cast<int>(i),
                       "\"" + word + "\" not in trie - nothing to remove.",
                       "REMOVE_FAIL", 3);
            ok = false; break;
        }
    }
    if (ok) {
        trie.remove(word);
        auto it = std::find(currentWords.begin(), currentWords.end(), word);
        if (it != currentWords.end()) currentWords.erase(it);
        // line 5: isEndOfWord = false, line 6: prune
        recordStep("", -1, "Done - \"" + word + "\" removed. Unused nodes pruned.",
                   "REMOVE_DONE", 5);
    }
    currentStep = 0;
}

bool TrieVisualizer::wordExists(const std::string& word) const {
    return trie.search(word);
}

// ── Layout ────────────────────────────────────────────────────────────────────
float TrieVisualizer::calculateSubtreeWidths(TrieNode* node,
                                              std::map<TrieNode*, float>& widths) {
    if (!node) return 0.f;
    float w = 0.f; int c = 0;
    for (int i = 0; i < 256; ++i)
        if (node->children[i]) { w += calculateSubtreeWidths(node->children[i], widths); ++c; }
    if (c == 0) w = 52.f; else w += 10.f * (c - 1);
    widths[node] = w;
    return w;
}

void TrieVisualizer::buildTreeLayout(
        TrieNode* node, float x, float y,
        const std::string& currentPath, const TrieStep& stepState,
        std::map<TrieNode*, float>& widths,
        float windowWidth, float windowHeight,
        std::map<std::string, TrieNodeRenderInfo>& outNodes,
        std::vector<std::pair<std::string,std::string>>& outEdges)
{
    if (!node) return;

    float cx = windowWidth / 2.f, cy = windowHeight / 2.f;
    sf::Vector2f pos(cx + (x - cx) * zoomLevel + panOffset.x,
                     cy + (y - cy) * zoomLevel + panOffset.y);

    bool isActive = false;
    if (stepState.activeCharIndex >= 0 && !stepState.activeWord.empty()) {
        isActive = (currentPath == stepState.activeWord.substr(0, stepState.activeCharIndex));
    } else if (stepState.activeCharIndex == -1 && currentPath.empty()) {
        isActive = true;
    }

    sf::Color fill, outline;
    const std::string& op = stepState.operation;
    if (isActive) {
        if      (op == "INSERT")       { fill = kColInsert;     outline = kOutActive;   }
        else if (op == "INSERT_DONE")  { fill = kColInsertDone; outline = kOutEndOfWord;}
        else if (op == "SEARCH")       { fill = kColSearch;     outline = kOutSearch;   }
        else if (op == "SEARCH_FOUND") { fill = kColFound;      outline = kOutFound;    }
        else if (op == "SEARCH_FAIL")  { fill = kColFail;       outline = kOutFail;     }
        else                           { fill = kColRemove;     outline = kOutRemove;   }
    } else if (node->isEndOfWord) {
        fill = kColEndOfWord; outline = kOutEndOfWord;
    } else {
        fill = kColNormal;    outline = kOutNormal;
    }

    TrieNodeRenderInfo info;
    info.targetPos    = pos;
    info.fillColor    = fill;
    info.outlineColor = outline;
    info.label        = currentPath.empty() ? "*" : std::string(1, currentPath.back());
    info.path         = currentPath;
    info.isEndOfWord  = node->isEndOfWord;
    info.isActive     = isActive;
    outNodes[currentPath] = info;

    float startX = x - widths[node] / 2.f;
    for (int i = 0; i < 256; ++i) {
        if (!node->children[i]) continue;
        float cw = widths[node->children[i]];
        float cx2 = startX + cw / 2.f;
        std::string childPath = currentPath + static_cast<char>(i);
        outEdges.push_back({ currentPath, childPath });
        buildTreeLayout(node->children[i], cx2, y + kLevelSpacing, childPath,
                        stepState, widths, windowWidth, windowHeight, outNodes, outEdges);
        startX += cw + 10.f;
    }
}

void TrieVisualizer::computeLayout(
        const TrieStep& state, float ww, float wh,
        std::map<std::string, TrieNodeRenderInfo>& outNodes,
        std::vector<std::pair<std::string,std::string>>& outEdges)
{
    outNodes.clear(); outEdges.clear();
    Trie tmp;
    for (const auto& w : state.words) tmp.insert(w);
    if (state.operation == "INSERT" && state.activeCharIndex > 0)
        tmp.insert(state.activeWord.substr(0, state.activeCharIndex));

    TrieNode* root = tmp.getRoot();
    if (!root) return;
    std::map<TrieNode*, float> widths;
    calculateSubtreeWidths(root, widths);
    buildTreeLayout(root, ww / 2.f, 200.f, "", state, widths, ww, wh, outNodes, outEdges);
}

// ── Node pool sync ────────────────────────────────────────────────────────────
void TrieVisualizer::syncNodePool(
        const std::map<std::string, TrieNodeRenderInfo>& layout,
        float baseRadius)
{
    for (auto& [path, n] : nodePool) n.targetAlpha = 0.f;

    for (const auto& [path, info] : layout) {
        if (nodePool.count(path) == 0) {
            AnimatedNode n;
            n.currentPos   = info.targetPos;
            n.targetPos    = info.targetPos;
            n.currentRadius= baseRadius;
            n.targetRadius = baseRadius;
            n.alpha        = 0.f;
            n.targetAlpha  = 255.f;
            n.fillColor    = info.fillColor;
            n.outlineColor = info.outlineColor;
            n.label        = info.label;
            n.path         = path;
            n.isActive     = info.isActive;
            nodePool[path] = n;
        } else {
            AnimatedNode& n = nodePool[path];
            n.targetPos     = info.targetPos;
            n.targetAlpha   = 255.f;
            n.fillColor     = info.fillColor;
            n.outlineColor  = info.outlineColor;
            n.label         = info.label;
            n.isActive      = info.isActive;
            n.targetRadius  = info.isActive ? baseRadius * 1.28f : baseRadius;
        }
    }

    for (auto it = nodePool.begin(); it != nodePool.end(); )
        it = (it->second.targetAlpha < 1.f && it->second.alpha < 1.f)
             ? nodePool.erase(it) : ++it;
}

// ── update() ──────────────────────────────────────────────────────────────────
void TrieVisualizer::update(float deltaTime) {
    globalPulse += deltaTime * 3.5f;

    for (auto& [path, n] : nodePool) {
        // Smooth position
        float t = clamp01(kSmoothSpeed * deltaTime);
        n.currentPos = lerpVec(n.currentPos, n.targetPos, t);

        // Fade alpha
        float da = kAlphaSpeed * deltaTime * 255.f;
        if (n.alpha < n.targetAlpha) n.alpha = std::min(n.targetAlpha, n.alpha + da);
        else                         n.alpha = std::max(n.targetAlpha, n.alpha - da);

        // Smooth radius
        float dr = kRadiusSpeed * deltaTime * 20.f;
        if (n.currentRadius < n.targetRadius) n.currentRadius = std::min(n.targetRadius, n.currentRadius + dr);
        else                                   n.currentRadius = std::max(n.targetRadius, n.currentRadius - dr);

        // Pulse phase
        if (n.isActive) n.pulsePhase += deltaTime * 4.f;
        else            n.pulsePhase = 0.f;
    }

    // Auto-advance steps
    if (autoRun && currentStep < static_cast<int>(steps.size()) - 1) {
        float stepDelay = 0.65f / std::max(0.1f, playbackSpeed);
        elapsedTime += deltaTime;
        if (elapsedTime >= stepDelay) { elapsedTime = 0.f; goToNextStep(); }
    }
    if (isAnimating && currentStep >= static_cast<int>(steps.size()) - 1)
        isAnimating = false;
}

// ── render() ─────────────────────────────────────────────────────────────────
void TrieVisualizer::render(sf::RenderWindow& window) {
    sf::Vector2u ws = window.getSize();
    float ww = static_cast<float>(ws.x), wh = static_cast<float>(ws.y);
    if (steps.empty()) return;

    const TrieStep& state = steps[currentStep];

    std::map<std::string, TrieNodeRenderInfo> layout;
    std::vector<std::pair<std::string,std::string>> edgeKeys;
    computeLayout(state, ww, wh, layout, edgeKeys);

    float baseRadius = kBaseRadius * zoomLevel;
    syncNodePool(layout, baseRadius);

    // Draw edges
    for (const auto& [fromPath, toPath] : edgeKeys) {
        auto itF = nodePool.find(fromPath);
        auto itT = nodePool.find(toPath);
        if (itF == nodePool.end() || itT == nodePool.end()) continue;

        bool active = itF->second.isActive || itT->second.isActive;
        float alpha = std::min(itF->second.alpha, itT->second.alpha);
        sf::Color ec = active ? kEdgeActive : kEdgeNormal;
        ec.a = static_cast<std::uint8_t>(std::min(255.f, alpha));

        sf::Vertex line[] = {
            sf::Vertex{ itF->second.currentPos, ec },
            sf::Vertex{ itT->second.currentPos, ec }
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }

    // Draw nodes
    for (auto& [path, n] : nodePool) {
        if (n.alpha < 2.f) continue;

        float r = n.currentRadius;
        if (n.isActive) r *= (1.f + 0.08f * std::sin(n.pulsePhase));

        UI::VisualNode vn(*font, n.label, r);
        vn.setPosition(n.currentPos);

        sf::Color fill    = n.fillColor;
        sf::Color outline = n.outlineColor;
        fill.a    = static_cast<std::uint8_t>(std::min(255.f, n.alpha));
        outline.a = fill.a;
        vn.setFillColor(fill);
        vn.setOutlineColor(outline);
        vn.render(window);
    }

    // Description is shown via VisualizationScene::displayStatus — no drawing needed here
}

// ── Playback ──────────────────────────────────────────────────────────────────
void TrieVisualizer::setPlaybackSpeed(float s) { playbackSpeed = std::max(0.1f, s); }
void TrieVisualizer::setAutoRun(bool v)        { autoRun = v; if (!autoRun) elapsedTime = 0.f; }
void TrieVisualizer::goToFirstStep()    { currentStep = 0; elapsedTime = 0.f; }
void TrieVisualizer::goToPreviousStep() { if (currentStep > 0) { --currentStep; elapsedTime = 0.f; } }
void TrieVisualizer::goToNextStep()     { if (currentStep < static_cast<int>(steps.size())-1) { ++currentStep; elapsedTime = 0.f; } }
void TrieVisualizer::goToFinalStep()    { if (!steps.empty()) { currentStep = static_cast<int>(steps.size())-1; elapsedTime = 0.f; } }

void TrieVisualizer::reset() {
    trie.clear();  // Use clear() instead of assignment to avoid shallow copy
    currentWords.clear(); 
    steps.clear();
    currentStep = 0;  // Reset to 0 when steps is empty
    elapsedTime = 0.f; 
    isAnimating = false;
    nodePool.clear(); 
    edgeList.clear();
    scanProgress = 0.f;
    isScanning = false;
    globalPulse = 0.f;
}

std::string TrieVisualizer::getProperties() const {
    std::ostringstream oss;
    oss << "Trie | Step " << (currentStep+1) << " / " << steps.size();
    if (currentStep < static_cast<int>(steps.size()))
        oss << "\n" << steps[currentStep].description;
    return oss.str();
}

void TrieVisualizer::processEvents(const sf::Event& event) {
    handleZoomPanEvents(event);
}
