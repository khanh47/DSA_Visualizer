#include "KruskalVisualizer.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

#include "ResourceManager.h"

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

    const auto& edges = graph.getEdges();
    if (nodeCount == 3) {
        const float twoPi = 6.28318530718f;
        const float radius = 170.0f;
        for (int i = 0; i < 3; ++i) {
            const float angle = twoPi * static_cast<float>(i) / 3.0f - 1.57079632679f;
            layoutPositions[static_cast<std::size_t>(i)] = {std::cos(angle) * radius, std::sin(angle) * radius};
        }
        return;
    }

    if (nodeCount == 5) {
        const float radius = 175.0f;
        const float twoPi = 6.28318530718f;
        for (int i = 0; i < 5; ++i) {
            const float angle = twoPi * static_cast<float>(i) / 5.0f - 1.57079632679f;
            layoutPositions[static_cast<std::size_t>(i)] = {std::cos(angle) * radius, std::sin(angle) * radius};
        }
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
    currentMstEdge = 0;
    animationElapsed = 0.0f;
    animationReady = false;
}

void KruskalVisualizer::run() {
    if (layoutPositions.size() != static_cast<std::size_t>(graph.getNumNodes())) {
        buildStaticLayout();
    }
    mstEdges = graph.runKruskal();
    currentMstEdge = 0;
    animationElapsed = 0.0f;
    animationReady = true;
    autoRun = true;
}

void KruskalVisualizer::reset() {
    graph.clear();
    layoutPositions.clear();
    mstEdges.clear();
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
    if (!animationReady || !autoRun || currentMstEdge >= mstEdges.size()) {
        return;
    }

    animationElapsed += deltaTime * playbackSpeed;
    while (animationElapsed >= 1.0f && currentMstEdge < mstEdges.size()) {
        animationElapsed -= 1.0f;
        ++currentMstEdge;
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
    const sf::Vector2f center(
        static_cast<float>(windowSize.x) * 0.5f,
        static_cast<float>(windowSize.y) * graphCenterYFactor
    );

    const float zoomTextScale = std::clamp(zoomLevel, 0.5f, 3.0f);
    const unsigned int edgeTextSize = static_cast<unsigned int>(std::clamp(18.0f * zoomTextScale, 10.0f, 48.0f));

    auto transformPosition = [&](const sf::Vector2f& base) {
        return sf::Vector2f(
            center.x + base.x * zoomLevel + panOffset.x,
            center.y + base.y * zoomLevel + panOffset.y
        );
    };

    std::vector<sf::Vector2f> nodePositions(static_cast<std::size_t>(nodeCount), center);
    for (int node = 0; node < nodeCount; ++node) {
        nodePositions[static_cast<std::size_t>(node)] = transformPosition(layoutPositions[static_cast<std::size_t>(node)]);
    }

    const std::size_t mstVisibleCount = std::min(currentMstEdge, mstEdges.size());

    for (const auto& edge : edges) {
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

        const sf::Color edgeColor = isSelected ? sf::Color(60, 120, 220) : sf::Color::Black;
        const float edgeThickness = isSelected ? 7.0f : 3.0f;
        UI::VisualEdge visualEdge(start, end, edgeThickness, edgeColor);
        visualEdge.render(window);

        sf::Text weightText(*font, std::to_string(edge.w), edgeTextSize);
        weightText.setFillColor(edgeColor);
        const sf::FloatRect bounds = weightText.getLocalBounds();
        weightText.setOrigin({
            bounds.position.x + bounds.size.x * 0.5f,
            bounds.position.y + bounds.size.y * 0.5f
        });

        weightText.setPosition(visualEdge.labelPosition(16.0f * zoomTextScale));
        window.draw(weightText);
    }

    std::vector<std::unique_ptr<UI::VisualNode>> nodes;
    nodes.reserve(static_cast<std::size_t>(nodeCount));

    for (int i = 0; i < nodeCount; ++i) {
        auto node = std::make_unique<UI::VisualNode>(*font, std::to_string(i), 26.0f * zoomLevel);
        node->setPosition(nodePositions[static_cast<std::size_t>(i)]);
        node->render(window);
        nodes.push_back(std::move(node));
    }
}

void KruskalVisualizer::goToFirstStep() {
    currentMstEdge = 0;
    animationElapsed = 0.0f;
}

void KruskalVisualizer::goToPreviousStep() {
    if (currentMstEdge > 0) {
        --currentMstEdge;
    }
    animationElapsed = 0.0f;
}

void KruskalVisualizer::goToNextStep() {
    if (currentMstEdge < mstEdges.size()) {
        ++currentMstEdge;
    }
    animationElapsed = 0.0f;
}

void KruskalVisualizer::goToFinalStep() {
    currentMstEdge = mstEdges.size();
    animationElapsed = 0.0f;
}