#include "KruskalVisualizer.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "ResourceManager.h"

namespace {

std::string makeVertexLabel(int index) {
    if (index >= 0 && index < 26) {
        return std::string(1, static_cast<char>('A' + index));
    }

    if (index >= 26) {
        return "V" + std::to_string(index);
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
    buildStaticLayout();
    mstEdges.clear();
    currentParents.clear();
    currentEdgeIndex = 0;
    currentMstEdge = 0;
    animationElapsed = 0.0f;
    animationReady = false;
}

void KruskalVisualizer::run() {
    if (!autoRun) return;
    if (layoutPositions.size() != static_cast<std::size_t>(graph.getNumNodes())) {
        buildStaticLayout();
    }
    mstEdges = graph.runKruskal();
    currentEdgeIndex = 0;
    animationElapsed = 0.0f;
    animationReady = true;
    autoRun = true;
    syncAlgorithmState();
}

void KruskalVisualizer::reset() {
    graph.clear();
    layoutPositions.clear();
    mstEdges.clear();
    currentParents.clear();
    currentEdgeIndex = 0;
    currentMstEdge = 0;
    animationElapsed = 0.0f;
    autoRun = false;
    playbackSpeed = 1.0f;
    animationReady = false;
}

std::string KruskalVisualizer::getProperties() const {
    return "Kruskal visualizer";
}

void KruskalVisualizer::setPlaybackSpeed(float speed) {
    playbackSpeed = std::max(0.1f, speed);
}

void KruskalVisualizer::setAutoRun(bool value) {
    autoRun = value;
}

void KruskalVisualizer::syncAlgorithmState() {
    const int nodeCount = graph.getNumNodes();
    currentParents.clear();
    currentMstEdge = 0;

    if (nodeCount <= 0) {
        return;
    }

    const auto& edges = graph.getEdges();
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
    if (!animationReady || !autoRun || currentEdgeIndex >= graph.getEdges().size()) {
        return;
    }

    animationElapsed += deltaTime * playbackSpeed;
    while (animationElapsed >= 1.0f && currentEdgeIndex < graph.getEdges().size()) {
        animationElapsed -= 1.0f;
        ++currentEdgeIndex;
        syncAlgorithmState();
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

    const auto& edges = graph.getEdges();
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
    const bool hasCurrentEdge = animationReady && displayedEdgeIndex < edges.size();
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
    std::unordered_map<int, int> rootToLabel;
    int nextLabel = 1;

    auto findRoot = [&](int node) {
        int root = node;
        while (root >= 0 && root < static_cast<int>(currentParents.size()) && currentParents[static_cast<std::size_t>(root)] >= 0) {
            root = currentParents[static_cast<std::size_t>(root)];
        }
        return root;
    };

    for (int node = 0; node < nodeCount; ++node) {
        const int root = findRoot(node);
        auto [it, inserted] = rootToLabel.emplace(root, nextLabel);
        if (inserted) {
            ++nextLabel;
        }
        componentLabels[static_cast<std::size_t>(node)] = it->second;
    }

    const float leftContentX = stateLeftX;
    const float leftContentY = stateTopY;
    const float rowHeight = 24.0f;

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

    drawText("Looking at disjoint set", 26, {leftContentX, leftContentY}, sf::Color(40, 50, 70), true);

    std::string findLine;
    if (hasCurrentEdge) {
        std::ostringstream findStream;
        findStream << "find(" << makeVertexLabel(currentEdge.u) << ") = " << componentLabels[static_cast<std::size_t>(currentEdge.u)]
                   << "    find(" << makeVertexLabel(currentEdge.v) << ") = " << componentLabels[static_cast<std::size_t>(currentEdge.v)];
        findLine = findStream.str();
    } else {
        findLine = "No current edge";
    }
    drawText(findLine, 20, {leftContentX, leftContentY + 45.0f}, sf::Color(30, 40, 60));

    drawText("Vertex      Set ID", 26, {leftContentX, leftContentY + 80.0f}, sf::Color(30, 40, 60), true);

    for (int node = 0; node < nodeCount; ++node) {
        const float rowY = leftContentY + 120.0f + static_cast<float>(node) * rowHeight;

        const bool isCurrentVertex = hasCurrentEdge && (node == currentEdge.u || node == currentEdge.v);
        const sf::Color rowColor = isCurrentVertex ? sf::Color(220, 120, 25) : sf::Color(30, 40, 60);
        drawText(makeVertexLabel(node), 20, {leftContentX + 10.0f, rowY}, rowColor);
        drawText(std::to_string(componentLabels[static_cast<std::size_t>(node)]), 20, {leftContentX + 116.0f, rowY}, rowColor);
    }

    const float queueX = leftContentX;
    const float queueY = leftContentY + 150.0f + static_cast<float>(nodeCount) * rowHeight;

    std::string currentEdgeLine;
    if (hasCurrentEdge) {
        currentEdgeLine = "Current edge: " + makeEdgeLabel(currentEdge) + "  " + std::to_string(currentEdge.w);
    } else {
        currentEdgeLine = "Current edge: done";
    }
    drawText(currentEdgeLine, 20, {queueX, queueY}, sf::Color(30, 40, 60));
    drawText("List of edges:", 20, {queueX, queueY + 30.0f}, sf::Color(30, 40, 60), true);

    const float queueStartY = queueY + 60.0f;
    for (std::size_t i = 0; i < edges.size(); ++i) {
        const auto& edge = edges[i];
        const bool isCurrentQueueEdge = hasCurrentEdge && i == displayedEdgeIndex;
        
        sf::Text edgeText(*font, makeEdgeLabel(edge) + "   " + std::to_string(edge.w), 20);
        edgeText.setFillColor(isCurrentQueueEdge ? sf::Color(220, 120, 25) : sf::Color(35, 45, 60));
        edgeText.setPosition({queueX, queueStartY + static_cast<float>(i) * 30.0f});

        if (isCurrentQueueEdge) {
            const sf::FloatRect textBounds = edgeText.getLocalBounds();
            const float paddingX = 6.0f;
            const float paddingY = 4.0f;
            sf::RectangleShape highlight({textBounds.size.x + paddingX * 2.0f, textBounds.size.y + paddingY * 2.0f});
            highlight.setPosition({
                edgeText.getPosition().x + textBounds.position.x - paddingX,
                edgeText.getPosition().y + textBounds.position.y - paddingY
            });
            highlight.setFillColor(sf::Color(255, 232, 206));
            highlight.setOutlineThickness(1.5f);
            highlight.setOutlineColor(sf::Color(232, 140, 40));
            window.draw(highlight);
        }
        window.draw(edgeText);
    }
}

void KruskalVisualizer::goToFirstStep() {
    currentEdgeIndex = 0;
    animationElapsed = 0.0f;
    syncAlgorithmState();
}

void KruskalVisualizer::goToPreviousStep() {
    if (currentEdgeIndex > 0) {
        --currentEdgeIndex;
    }
    animationElapsed = 0.0f;
    syncAlgorithmState();
}

void KruskalVisualizer::goToNextStep() {
    if (currentEdgeIndex < graph.getEdges().size()) {
        ++currentEdgeIndex;
    }
    animationElapsed = 0.0f;
    syncAlgorithmState();
}

void KruskalVisualizer::goToFinalStep() {
    currentEdgeIndex = graph.getEdges().size();
    animationElapsed = 0.0f;
    syncAlgorithmState();
}