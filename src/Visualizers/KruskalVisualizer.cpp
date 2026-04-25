#include "KruskalVisualizer.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "ResourceManager.h"
#include "RoundedRect.h"

namespace {

constexpr float kStepDurationSeconds = 1.6f;
constexpr float kManualPseudoStepDurationSeconds = 0.9f;

std::string makeVertexLabel(int index) {
    if (index >= 0) {
        return std::to_string(index);
    }

    return "?";
}

std::string makeEdgeLabel(const KruskalDataStructure::Edge& edge) {
    return makeVertexLabel(edge.u) + " - " + makeVertexLabel(edge.v);
}

}

KruskalVisualizer::KruskalVisualizer() {
    font = &ResourceManager::getInstance().getFont("Roboto");
    random();
}

void KruskalVisualizer::rebuildSortedEdges() {
    sortedEdges = graph.getEdges();
}

void KruskalVisualizer::buildStaticLayout() {
    const int nodeCount = graph.getNumNodes();
    layoutPositions.clear();
    layoutPositions.resize(static_cast<std::size_t>(nodeCount), {0.0f, 0.0f});

    if (nodeCount <= 0) {
        return;
    }

    const auto placeRegularPolygon = [&](int n, float radius) {
        const float twoPi = 6.28318530718f;
        for (int i = 0; i < n; ++i) {
            const float angle = twoPi * static_cast<float>(i) / static_cast<float>(n) - 1.57079632679f;
            layoutPositions[static_cast<std::size_t>(i)] = {std::cos(angle) * radius, std::sin(angle) * radius};
        }
    };

    const auto& edges = graph.getEdges();
    if (nodeCount == 3) {
        placeRegularPolygon(3, 170.0f);
        return;
    }
    if (nodeCount == 5) {
        placeRegularPolygon(5, 175.0f);
        return;
    }

    std::vector<int> degrees(static_cast<std::size_t>(nodeCount), 0);
    for (const auto& edge : edges) {
        if (edge.u >= 0 && edge.u < nodeCount) {
            ++degrees[static_cast<std::size_t>(edge.u)];
        }
        if (edge.v >= 0 && edge.v < nodeCount) {
            ++degrees[static_cast<std::size_t>(edge.v)];
        }
    }

    int hub = 0;
    for (int i = 1; i < nodeCount; ++i) {
        if (degrees[static_cast<std::size_t>(i)] > degrees[static_cast<std::size_t>(hub)]) {
            hub = i;
        }
    }

    layoutPositions[static_cast<std::size_t>(hub)] = {0.0f, 0.0f};
    const float radius = std::max(150.0f, 45.0f * static_cast<float>(nodeCount));
    const float twoPi = 6.28318530718f;
    int ringIndex = 0;

    for (int i = 0; i < nodeCount; ++i) {
        if (i == hub) {
            continue;
        }
        const float angle = twoPi * static_cast<float>(ringIndex++) / std::max(1, nodeCount - 1) - 1.57079632679f;
        layoutPositions[static_cast<std::size_t>(i)] = {std::cos(angle) * radius, std::sin(angle) * radius};
    }
}

void KruskalVisualizer::random() {
    graph.randomGraph();
    rebuildSortedEdges();
    buildStaticLayout();
    mstEdges.clear();
    currentParents.clear();
    currentEdgeIndex = 0;
    currentMstEdge = 0;
    animationElapsed = 0.0f;
    manualPseudoElapsed = 0.0f;
    animationReady = false;
    manualPseudoAnimating = false;
    manualAdvancePending = false;
    manualEdgeVisible = false;
    manualPseudoStartLine = -1;
    manualPseudoEndLine = -1;
}

void KruskalVisualizer::run() {
    if (layoutPositions.size() != static_cast<std::size_t>(graph.getNumNodes())) {
        buildStaticLayout();
    }
    rebuildSortedEdges();
    std::sort(sortedEdges.begin(), sortedEdges.end(), [](const auto& a, const auto& b) {
        if (a.w != b.w) {
            return a.w < b.w;
        }
        if (a.u != b.u) {
            return a.u < b.u;
        }
        return a.v < b.v;
    });
    mstEdges = graph.runKruskal();
    currentEdgeIndex = 0;
    animationElapsed = 0.0f;
    manualPseudoElapsed = 0.0f;
    animationReady = true;
    manualAdvancePending = false;
    syncAlgorithmState();

    if (autoRun) {
        manualPseudoAnimating = false;
        manualEdgeVisible = true;
        manualPseudoStartLine = -1;
        manualPseudoEndLine = -1;
    } else {
        // Manual Run only plays intro pseudocode; no edge is being checked yet.
        manualPseudoAnimating = true;
        manualEdgeVisible = false;
        manualPseudoStartLine = 0;
        manualPseudoEndLine = 1;
    }
}

void KruskalVisualizer::reset() {
    graph.clear();
    sortedEdges.clear();
    layoutPositions.clear();
    mstEdges.clear();
    currentParents.clear();
    currentEdgeIndex = 0;
    currentMstEdge = 0;
    animationElapsed = 0.0f;
    manualPseudoElapsed = 0.0f;
    autoRun = false;
    playbackSpeed = 1.0f;
    animationReady = false;
    manualPseudoAnimating = false;
    manualAdvancePending = false;
    manualEdgeVisible = false;
    manualPseudoStartLine = -1;
    manualPseudoEndLine = -1;
}

std::string KruskalVisualizer::getProperties() const {
    return "Kruskal visualizer";
}

void KruskalVisualizer::setPlaybackSpeed(float speed) {
    playbackSpeed = std::max(0.1f, speed);
}

void KruskalVisualizer::setAutoRun(bool value) {
    autoRun = value;
    if (!autoRun) {
        animationElapsed = 0.0f;
        manualEdgeVisible = false;
    } else {
        manualPseudoAnimating = false;
        manualAdvancePending = false;
        manualEdgeVisible = animationReady;
        manualPseudoStartLine = -1;
        manualPseudoEndLine = -1;
    }
}

void KruskalVisualizer::syncAlgorithmState() {
    const int nodeCount = graph.getNumNodes();
    currentParents.clear();
    currentMstEdge = 0;

    if (nodeCount <= 0) {
        return;
    }

    const auto& edges = sortedEdges;
    const std::size_t consideredCount = std::min(currentEdgeIndex, edges.size());

    KruskalDataStructure::DSU dsu;
    dsu.init(nodeCount);
    for (std::size_t i = 0; i < consideredCount; ++i) {
        if (dsu.join(edges[i].u, edges[i].v)) {
            ++currentMstEdge;
        }
    }

    currentParents = std::move(dsu.lab);
}

KruskalDataStructure& KruskalVisualizer::data() {
    return graph;
}

const KruskalDataStructure& KruskalVisualizer::data() const {
    return graph;
}

void KruskalVisualizer::processEvents(const sf::Event& event) {
    handleZoomPanEvents(event);
}

void KruskalVisualizer::update(float deltaTime) {
    const auto& edges = sortedEdges;
    if (!animationReady) {
        return;
    }

    if (!autoRun) {
        if (manualPseudoAnimating) {
            manualPseudoElapsed += deltaTime * playbackSpeed;
            if (manualPseudoElapsed >= kManualPseudoStepDurationSeconds) {
                manualPseudoElapsed = kManualPseudoStepDurationSeconds;
                manualPseudoAnimating = false;

                if (manualAdvancePending) {
                    if (currentEdgeIndex < edges.size()) {
                        ++currentEdgeIndex;
                    }
                    syncAlgorithmState();

                    const std::size_t targetMstEdges = static_cast<std::size_t>(std::max(0, graph.getNumNodes() - 1));
                    if (currentMstEdge >= targetMstEdges) {
                        currentEdgeIndex = edges.size();
                    }

                    // In manual mode, only show orange while actively checking.
                    // After a step resolves, keep MST edges blue and hide the next edge
                    // until the user presses ">" again.
                    manualEdgeVisible = false;
                    manualAdvancePending = false;
                }
            }
        }
        return;
    }

    if (currentEdgeIndex >= edges.size()) {
        return;
    }

    const std::size_t targetMstEdges = static_cast<std::size_t>(std::max(0, graph.getNumNodes() - 1));

    animationElapsed += deltaTime * playbackSpeed;
    while (animationElapsed >= kStepDurationSeconds && currentEdgeIndex < edges.size()) {
        animationElapsed -= kStepDurationSeconds;
        ++currentEdgeIndex;
        syncAlgorithmState();

        // Kruskal stops once MST has n-1 edges. End playback immediately.
        if (currentMstEdge >= targetMstEdges) {
            currentEdgeIndex = edges.size();
            animationElapsed = 0.0f;
            break;
        }
    }
}

void KruskalVisualizer::render(sf::RenderWindow& window) {
    if (!font) {
        return;
    }

    const int nodeCount = graph.getNumNodes();
    if (nodeCount <= 0) {
        return;
    }

    if (layoutPositions.size() != static_cast<std::size_t>(nodeCount)) {
        buildStaticLayout();
    }

    const auto& edges = sortedEdges;
    const sf::Vector2u windowSize = window.getSize();
    const float graphCenterYFactor = 0.54f;
    const float stateLeftX = 22.0f;
    const float stateTopY = 170.0f;
    const sf::Vector2f center(
        static_cast<float>(windowSize.x) * 0.5f,
        static_cast<float>(windowSize.y) * graphCenterYFactor
    );

    const float zoomTextScale = std::clamp(zoomLevel, 0.5f, 3.0f);
    const unsigned int edgeTextSize = static_cast<unsigned int>(std::clamp(16.0f * zoomTextScale, 9.0f, 42.0f));

    auto transformPosition = [&](const sf::Vector2f& base) {
        return sf::Vector2f(
            center.x + base.x * zoomLevel + panOffset.x,
            center.y + base.y * zoomLevel + panOffset.y
        );
    };

    if (currentParents.empty() && !edges.empty()) {
        syncAlgorithmState();
    }

    std::vector<sf::Vector2f> nodePositions(static_cast<std::size_t>(nodeCount), center);
    for (int node = 0; node < nodeCount; ++node) {
        nodePositions[static_cast<std::size_t>(node)] = transformPosition(layoutPositions[static_cast<std::size_t>(node)]);
    }

    const std::size_t mstVisibleCount = std::min(currentMstEdge, mstEdges.size());
    const std::size_t displayedEdgeIndex = std::min(currentEdgeIndex, edges.size());
    const bool hasCurrentEdge = animationReady && displayedEdgeIndex < edges.size() && (autoRun || manualEdgeVisible);
    const KruskalDataStructure::Edge currentEdge = hasCurrentEdge ? edges[displayedEdgeIndex] : KruskalDataStructure::Edge{};
    std::vector<sf::FloatRect> placedWeightBoxes;

    auto boxesOverlap = [](const sf::FloatRect& a, const sf::FloatRect& b) {
        const float aLeft = a.position.x;
        const float aTop = a.position.y;
        const float aRight = a.position.x + a.size.x;
        const float aBottom = a.position.y + a.size.y;

        const float bLeft = b.position.x;
        const float bTop = b.position.y;
        const float bRight = b.position.x + b.size.x;
        const float bBottom = b.position.y + b.size.y;

        return !(aRight < bLeft || bRight < aLeft || aBottom < bTop || bBottom < aTop);
    };

    auto pointSegmentDistance = [](const sf::Vector2f& p, const sf::Vector2f& a, const sf::Vector2f& b) {
        const sf::Vector2f ab(b.x - a.x, b.y - a.y);
        const sf::Vector2f ap(p.x - a.x, p.y - a.y);
        const float ab2 = ab.x * ab.x + ab.y * ab.y;
        if (ab2 <= 0.001f) {
            const float dx = p.x - a.x;
            const float dy = p.y - a.y;
            return std::sqrt(dx * dx + dy * dy);
        }

        float t = (ap.x * ab.x + ap.y * ab.y) / ab2;
        t = std::clamp(t, 0.0f, 1.0f);
        const sf::Vector2f closest(a.x + ab.x * t, a.y + ab.y * t);
        const float dx = p.x - closest.x;
        const float dy = p.y - closest.y;
        return std::sqrt(dx * dx + dy * dy);
    };

    for (std::size_t edgeIndex = 0; edgeIndex < edges.size(); ++edgeIndex) {
        const auto& edge = edges[edgeIndex];
        if (edge.u < 0 || edge.v < 0 || edge.u >= nodeCount || edge.v >= nodeCount) {
            continue;
        }

        const sf::Vector2f start = nodePositions[static_cast<std::size_t>(edge.u)];
        const sf::Vector2f end = nodePositions[static_cast<std::size_t>(edge.v)];

        const bool isSelected = std::any_of(
            mstEdges.begin(),
            mstEdges.begin() + static_cast<std::ptrdiff_t>(mstVisibleCount),
            [&edge](const KruskalDataStructure::Edge& mstEdge) {
                return mstEdge.u == edge.u && mstEdge.v == edge.v && mstEdge.w == edge.w;
            }
        );

        const bool isCurrentEdge = hasCurrentEdge && edge.u == currentEdge.u && edge.v == currentEdge.v && edge.w == currentEdge.w;
        sf::Color edgeColor = sf::Color::Black;
        float edgeThickness = 3.0f;
        if (isSelected) {
            edgeColor = sf::Color(60, 120, 220);
            edgeThickness = 7.0f;
        }
        if (isCurrentEdge) {
            edgeColor = sf::Color(235, 140, 40);
            edgeThickness = 8.0f;
        }
        UI::VisualEdge visualEdge(start, end, edgeThickness, edgeColor);
        visualEdge.render(window);

        sf::Text weightText(*font, std::to_string(edge.w), edgeTextSize);
        weightText.setFillColor(edgeColor);
        const sf::FloatRect bounds = weightText.getLocalBounds();
        weightText.setOrigin({
            bounds.position.x + bounds.size.x * 0.5f,
            bounds.position.y + bounds.size.y * 0.5f
        });

        const float baseOffset = 16.0f * zoomTextScale;
        const float stepOffset = 12.0f * zoomTextScale;
        const float labelPadding = 2.5f * zoomTextScale;
        const float preferredSide = ((edge.u + edge.v) % 2 == 0) ? 1.0f : -1.0f;
        const float nodeAvoidRadius = 36.0f * zoomLevel;

        sf::Vector2f dir(end.x - start.x, end.y - start.y);
        const float dirLen = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (dirLen > 0.001f) {
            dir.x /= dirLen;
            dir.y /= dirLen;
        } else {
            dir = {1.0f, 0.0f};
        }

        const float tangentShifts[3] = {-18.0f * zoomTextScale, 0.0f, 18.0f * zoomTextScale};

        sf::Vector2f chosenPos = visualEdge.labelPosition(baseOffset, preferredSide);
        sf::FloatRect chosenBox{};
        float bestPenalty = std::numeric_limits<float>::max();

        for (int ring = 0; ring < 5; ++ring) {
            const float offset = baseOffset + stepOffset * static_cast<float>(ring);
            for (int sidePick = 0; sidePick < 2; ++sidePick) {
                const float side = sidePick == 0 ? preferredSide : -preferredSide;
                const sf::Vector2f sideBase = visualEdge.labelPosition(offset, side);
                for (float tangentShift : tangentShifts) {
                    const sf::Vector2f candidatePos(
                        sideBase.x + dir.x * tangentShift,
                        sideBase.y + dir.y * tangentShift
                    );

                    weightText.setPosition(candidatePos);
                    sf::FloatRect candidateBox = weightText.getGlobalBounds();
                    candidateBox.position.x -= labelPadding;
                    candidateBox.position.y -= labelPadding;
                    candidateBox.size.x += labelPadding * 2.0f;
                    candidateBox.size.y += labelPadding * 2.0f;

                    float penalty = 0.0f;

                    for (const sf::FloatRect& placed : placedWeightBoxes) {
                        if (boxesOverlap(candidateBox, placed)) {
                            penalty += 1000.0f;
                        }
                    }

                    const sf::Vector2f boxCenter(
                        candidateBox.position.x + candidateBox.size.x * 0.5f,
                        candidateBox.position.y + candidateBox.size.y * 0.5f
                    );

                    for (const sf::Vector2f& nodePos : nodePositions) {
                        const float dx = boxCenter.x - nodePos.x;
                        const float dy = boxCenter.y - nodePos.y;
                        const float d = std::sqrt(dx * dx + dy * dy);
                        if (d < nodeAvoidRadius) {
                            penalty += (nodeAvoidRadius - d) * 35.0f;
                        }
                    }

                    for (std::size_t otherIndex = 0; otherIndex < edges.size(); ++otherIndex) {
                        if (otherIndex == edgeIndex) {
                            continue;
                        }
                        const auto& other = edges[otherIndex];
                        if (other.u < 0 || other.v < 0 || other.u >= nodeCount || other.v >= nodeCount) {
                            continue;
                        }
                        const sf::Vector2f otherStart = nodePositions[static_cast<std::size_t>(other.u)];
                        const sf::Vector2f otherEnd = nodePositions[static_cast<std::size_t>(other.v)];
                        const float d = pointSegmentDistance(boxCenter, otherStart, otherEnd);
                        if (d < 20.0f * zoomTextScale) {
                            penalty += (20.0f * zoomTextScale - d) * 18.0f;
                        }
                    }

                    penalty += static_cast<float>(ring) * 0.2f;

                    if (penalty < bestPenalty) {
                        bestPenalty = penalty;
                        chosenPos = candidatePos;
                        chosenBox = candidateBox;
                    }
                }
            }
        }

        weightText.setPosition(chosenPos);
        placedWeightBoxes.push_back(chosenBox);
        window.draw(weightText);
    }

    for (int i = 0; i < nodeCount; ++i) {
        UI::VisualNode node(*font, std::to_string(i), 26.0f * zoomLevel);
        node.setPosition(nodePositions[static_cast<std::size_t>(i)]);
        node.render(window);
    }

    if (currentParents.empty()) {
        return;
    }

    std::vector<int> componentLabels(static_cast<std::size_t>(nodeCount), 0);

    auto findRoot = [&](int node) {
        int root = node;
        while (root >= 0 && root < static_cast<int>(currentParents.size()) && currentParents[static_cast<std::size_t>(root)] >= 0) {
            root = currentParents[static_cast<std::size_t>(root)];
        }
        return root;
    };

    for (int node = 0; node < nodeCount; ++node) {
        componentLabels[static_cast<std::size_t>(node)] = findRoot(node);
    }

    const float leftContentX = stateLeftX;
    const float leftContentY = stateTopY;
    const float panelWidth = 340.0f;
    const float panelGap = 14.0f;
    const float rowHeight = 30.0f;
    const float cornerRadius = 10.0f;

    const sf::Color panelBg(243, 246, 252, 236);
    const sf::Color panelOutline(160, 176, 205, 210);
    const sf::Color titleColor(37, 49, 74);
    const sf::Color textColor(44, 57, 82);
    const sf::Color gridColor(187, 199, 222, 180);
    const sf::Color headerBg(226, 234, 248, 220);
    const sf::Color altRowBg(238, 244, 255, 90);
    const sf::Color currentRowBg(255, 232, 206, 220);
    const sf::Color currentRowText(220, 120, 25);

    const auto drawText = [&](const std::string& text,
                              unsigned int size,
                              sf::Vector2f pos,
                              const sf::Color& color,
                              bool bold = false) {
        sf::Text t(*font, text, size);
        t.setFillColor(color);
        if (bold) {
            t.setStyle(sf::Text::Bold);
        }
        t.setPosition(pos);
        window.draw(t);
    };

    const auto drawPanel = [&](const sf::Vector2f& pos, const sf::Vector2f& size) {
        sf::ConvexShape panel = UI::makeRoundedRect(size, cornerRadius);
        panel.setPosition(pos);
        panel.setFillColor(panelBg);
        panel.setOutlineThickness(1.5f);
        panel.setOutlineColor(panelOutline);
        window.draw(panel);
    };

    std::string findLine;
    if (hasCurrentEdge) {
        std::ostringstream findStream;
        findStream << "find(" << makeVertexLabel(currentEdge.u) << ") = " << componentLabels[static_cast<std::size_t>(currentEdge.u)]
                   << "    find(" << makeVertexLabel(currentEdge.v) << ") = " << componentLabels[static_cast<std::size_t>(currentEdge.v)];
        findLine = findStream.str();
    } else {
        findLine = "No current edge";
    }

    std::string currentEdgeLine;
    if (hasCurrentEdge) {
        currentEdgeLine = "Current edge: " + makeEdgeLabel(currentEdge) + "  " + std::to_string(currentEdge.w);
    } else {
        currentEdgeLine = "Current edge: done";
    }

    // Status panel
    const float statusHeight = 92.0f;
    drawPanel({leftContentX, leftContentY}, {panelWidth, statusHeight});
    drawText("Looking at disjoint set", 24, {leftContentX + 14.0f, leftContentY + 10.0f}, titleColor, true);
    drawText(findLine, 17, {leftContentX + 14.0f, leftContentY + 52.0f}, textColor);

    // Vertex / set-id table panel
    const float dsuPanelY = leftContentY + statusHeight + panelGap;
    const float dsuTitleBlockHeight = 34.0f;
    const float dsuHeaderHeight = 30.0f;
    const float dsuTableHeight = dsuHeaderHeight + static_cast<float>(nodeCount) * rowHeight;
    const float dsuPanelHeight = dsuTitleBlockHeight + dsuTableHeight + 14.0f;
    drawPanel({leftContentX, dsuPanelY}, {panelWidth, dsuPanelHeight});
    drawText("Vertex / Set ID", 19, {leftContentX + 14.0f, dsuPanelY + 8.0f}, titleColor, true);

    const float dsuTableX = leftContentX + 10.0f;
    const float dsuTableY = dsuPanelY + dsuTitleBlockHeight;
    const float dsuTableW = panelWidth - 20.0f;
    const float dsuSplitX = dsuTableX + dsuTableW * 0.54f;

    sf::RectangleShape dsuHeaderBg({dsuTableW, dsuHeaderHeight});
    dsuHeaderBg.setPosition({dsuTableX, dsuTableY});
    dsuHeaderBg.setFillColor(headerBg);
    window.draw(dsuHeaderBg);

    drawText("Vertex", 16, {dsuTableX + 10.0f, dsuTableY + 6.0f}, titleColor, true);
    drawText("Set ID", 16, {dsuSplitX + 10.0f, dsuTableY + 6.0f}, titleColor, true);

    sf::RectangleShape dsuVertical({1.0f, dsuTableHeight});
    dsuVertical.setPosition({dsuSplitX, dsuTableY});
    dsuVertical.setFillColor(gridColor);
    window.draw(dsuVertical);

    for (int node = 0; node < nodeCount; ++node) {
        const float rowY = dsuTableY + dsuHeaderHeight + static_cast<float>(node) * rowHeight;
        const bool isCurrentVertex = hasCurrentEdge && (node == currentEdge.u || node == currentEdge.v);

        sf::RectangleShape rowBg({dsuTableW, rowHeight});
        rowBg.setPosition({dsuTableX, rowY});
        rowBg.setFillColor(isCurrentVertex ? currentRowBg : (node % 2 == 0 ? altRowBg : sf::Color::Transparent));
        window.draw(rowBg);

        sf::RectangleShape rowLine({dsuTableW, 1.0f});
        rowLine.setPosition({dsuTableX, rowY});
        rowLine.setFillColor(gridColor);
        window.draw(rowLine);

        const sf::Color rowTextColor = isCurrentVertex ? currentRowText : textColor;
        drawText(makeVertexLabel(node), 18, {dsuTableX + 10.0f, rowY + 5.0f}, rowTextColor);
        drawText(std::to_string(componentLabels[static_cast<std::size_t>(node)]), 18, {dsuSplitX + 10.0f, rowY + 5.0f}, rowTextColor);
    }

    // Edge list panel
    const float edgePanelY = dsuPanelY + dsuPanelHeight + panelGap;
    const float edgePanelBottom = std::max(edgePanelY + 200.0f, dragRegionBottomY - 10.0f);
    const float edgePanelHeight = edgePanelBottom - edgePanelY;
    drawPanel({leftContentX, edgePanelY}, {panelWidth, edgePanelHeight});
    drawText("List of edges", 19, {leftContentX + 14.0f, edgePanelY + 8.0f}, titleColor, true);
    drawText(currentEdgeLine, 16, {leftContentX + 14.0f, edgePanelY + 34.0f}, textColor);

    const float edgeTableX = leftContentX + 10.0f;
    const float edgeTableY = edgePanelY + 62.0f;
    const float edgeTableW = panelWidth - 20.0f;
    const float edgeHeaderHeight = 30.0f;
    const float edgeSplitX = edgeTableX + edgeTableW * 0.72f;

    sf::RectangleShape edgeHeaderBg({edgeTableW, edgeHeaderHeight});
    edgeHeaderBg.setPosition({edgeTableX, edgeTableY});
    edgeHeaderBg.setFillColor(headerBg);
    window.draw(edgeHeaderBg);

    drawText("Edge", 16, {edgeTableX + 10.0f, edgeTableY + 6.0f}, titleColor, true);
    drawText("Weight", 16, {edgeSplitX + 10.0f, edgeTableY + 6.0f}, titleColor, true);

    const float edgeRowsArea = std::max(0.0f, edgePanelHeight - 62.0f - edgeHeaderHeight - 10.0f);
    const std::size_t rowsToDraw = edges.size();
    const float edgeRowHeight = rowsToDraw > 0
        ? std::max(1.0f, edgeRowsArea / static_cast<float>(rowsToDraw))
        : rowHeight;
    const unsigned int edgeRowTextSize = static_cast<unsigned int>(
        std::clamp(edgeRowHeight * 0.58f, 10.0f, 18.0f)
    );
    const float edgeRowTextOffset = std::max(1.0f, (edgeRowHeight - static_cast<float>(edgeRowTextSize)) * 0.48f);

    sf::RectangleShape edgeVertical({1.0f, edgeHeaderHeight + static_cast<float>(rowsToDraw) * edgeRowHeight});
    edgeVertical.setPosition({edgeSplitX, edgeTableY});
    edgeVertical.setFillColor(gridColor);
    window.draw(edgeVertical);

    for (std::size_t i = 0; i < rowsToDraw; ++i) {
        const auto& edge = edges[i];
        const bool isCurrentQueueEdge = hasCurrentEdge && i == displayedEdgeIndex;
        const float rowY = edgeTableY + edgeHeaderHeight + static_cast<float>(i) * edgeRowHeight;

        sf::RectangleShape rowBg({edgeTableW, edgeRowHeight});
        rowBg.setPosition({edgeTableX, rowY});
        rowBg.setFillColor(isCurrentQueueEdge ? currentRowBg : (i % 2 == 0 ? altRowBg : sf::Color::Transparent));
        window.draw(rowBg);

        sf::RectangleShape rowLine({edgeTableW, 1.0f});
        rowLine.setPosition({edgeTableX, rowY});
        rowLine.setFillColor(gridColor);
        window.draw(rowLine);

        const sf::Color rowTextColor = isCurrentQueueEdge ? currentRowText : textColor;
        drawText(makeEdgeLabel(edge), edgeRowTextSize, {edgeTableX + 10.0f, rowY + edgeRowTextOffset}, rowTextColor);
        drawText(std::to_string(edge.w), edgeRowTextSize, {edgeSplitX + 10.0f, rowY + edgeRowTextOffset}, rowTextColor);
    }
}

void KruskalVisualizer::goToFirstStep() {
    currentEdgeIndex = 0;
    animationElapsed = 0.0f;
    manualPseudoElapsed = 0.0f;
    manualPseudoAnimating = false;
    manualAdvancePending = false;
    manualPseudoStartLine = -1;
    manualPseudoEndLine = -1;
    manualEdgeVisible = false;
    syncAlgorithmState();
}

void KruskalVisualizer::goToPreviousStep() {
    if (currentEdgeIndex > 0) {
        --currentEdgeIndex;
    }
    animationElapsed = 0.0f;
    manualPseudoElapsed = 0.0f;
    manualPseudoAnimating = false;
    manualAdvancePending = false;
    manualPseudoStartLine = -1;
    manualPseudoEndLine = -1;
    manualEdgeVisible = false;
    syncAlgorithmState();
}

void KruskalVisualizer::goToNextStep() {
    if (!animationReady) {
        return;
    }

    if (!autoRun) {
        if (manualPseudoAnimating || currentEdgeIndex >= sortedEdges.size()) {
            return;
        }

        auto findRoot = [&](int node) {
            int root = node;
            while (root >= 0 && root < static_cast<int>(currentParents.size()) && currentParents[static_cast<std::size_t>(root)] >= 0) {
                root = currentParents[static_cast<std::size_t>(root)];
            }
            return root;
        };

        const auto& edge = sortedEdges[currentEdgeIndex];
        const bool canJoin = findRoot(edge.u) != findRoot(edge.v);
        const std::size_t targetMstEdges = static_cast<std::size_t>(std::max(0, graph.getNumNodes() - 1));
        const bool finishesMst = canJoin && (currentMstEdge + 1 >= targetMstEdges);

        manualPseudoStartLine = 2;
        manualPseudoEndLine = !canJoin ? 3 : (finishesMst ? 5 : 4);
        manualPseudoElapsed = 0.0f;
        manualPseudoAnimating = true;
        manualAdvancePending = true;
        manualEdgeVisible = true;
        return;
    }

    if (currentEdgeIndex < sortedEdges.size()) {
        ++currentEdgeIndex;
    }
    animationElapsed = 0.0f;
    syncAlgorithmState();
}

void KruskalVisualizer::goToFinalStep() {
    currentEdgeIndex = sortedEdges.size();
    animationElapsed = 0.0f;
    manualPseudoElapsed = 0.0f;
    manualPseudoAnimating = false;
    manualAdvancePending = false;
    manualPseudoStartLine = -1;
    manualPseudoEndLine = -1;
    manualEdgeVisible = false;
    syncAlgorithmState();
}

int KruskalVisualizer::getCurrentPseudocodeLine() const {
    const auto& edges = sortedEdges;
    const int nodeCount = graph.getNumNodes();

    if (!animationReady || edges.empty() || nodeCount <= 0) {
        return -1;
    }

    if (currentEdgeIndex >= edges.size()) {
        return 5;
    }

    if (currentParents.empty()) {
        return 2;
    }

    if (!autoRun) {
        if (manualPseudoAnimating && manualPseudoStartLine >= 0 && manualPseudoEndLine >= manualPseudoStartLine) {
            const float t = std::clamp(manualPseudoElapsed / kManualPseudoStepDurationSeconds, 0.0f, 0.999f);
            const int span = manualPseudoEndLine - manualPseudoStartLine + 1;
            return manualPseudoStartLine + static_cast<int>(t * static_cast<float>(span));
        }

        if (currentEdgeIndex >= edges.size()) {
            return 5;
        }

        if (!manualEdgeVisible) {
            return 1;
        }

        return 2;
    }

    // Animate the highlight within each algorithm step so it advances downward
    // through the pseudocode instead of jumping directly to a target line.
    const float phase = autoRun
        ? std::clamp(animationElapsed / kStepDurationSeconds, 0.0f, 0.999f)
        : 1.0f;

    // Intro for the very first step: show setup lines before entering loop body.
    if (autoRun && currentEdgeIndex == 0) {
        if (phase < 0.20f) return 0;
        if (phase < 0.40f) return 1;
    }

    if (phase < 0.58f) {
        return 2;
    }

    if (phase < 0.78f) {
        return 3;
    }

    const auto& edge = edges[currentEdgeIndex];
    auto findRoot = [&](int node) {
        int root = node;
        while (root >= 0 && root < static_cast<int>(currentParents.size()) && currentParents[static_cast<std::size_t>(root)] >= 0) {
            root = currentParents[static_cast<std::size_t>(root)];
        }
        return root;
    };

    const bool canJoin = findRoot(edge.u) != findRoot(edge.v);
    if (!canJoin) {
        return 3;
    }

    if (currentMstEdge + 1 >= static_cast<std::size_t>(std::max(0, nodeCount - 1))) {
        if (phase < 0.92f) {
            return 4;
        }
        return 5;
    }

    return 4;
}