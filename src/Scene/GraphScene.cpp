#include "GraphScene.h"
#include "ResourceManager.h"
#include <algorithm>
#include <cctype>
#include <iostream>

namespace {
bool isPositiveInteger(const std::string& value) {
    if (value.empty()) {
        return false;
    }

    return std::all_of(value.begin(), value.end(), [](unsigned char ch) {
        return std::isdigit(ch) != 0;
    });
}

std::string toLowerCopy(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return value;
}
} // namespace

GraphScene::GraphScene(SceneManager& sceneManager)
    : VisualizationScene(sceneManager) {
    initializeOperationPanel();
}

void GraphScene::onVertex(const std::string& value) {
    if (value.empty()) {
        displayStatus("Vertex name is required.");
        return;
    }

    displayStatus("Added vertex: " + value);
}

void GraphScene::onEdge(const std::string& from, const std::string& to, const std::string& cost) {
    if (from.empty() || to.empty() || cost.empty()) {
        displayStatus("Edge requires vertex1, vertex2 and cost.");
        return;
    }

    displayStatus("Added edge: " + from + " - " + to + " (cost " + cost + ")");
}

void GraphScene::onRandom(const std::string& value) {
    if (!isPositiveInteger(value)) {
        displayStatus("Random requires a positive node count.");
        return;
    }

    displayStatus("Random graph with " + value + " nodes.");
}

void GraphScene::onRun(const std::string& algorithmValue) {
    const std::string algorithm = toLowerCopy(algorithmValue);
    if (algorithm != "prim" && algorithm != "kruskal") {
        displayStatus("Choose algorithm: prim or kruskal.");
        return;
    }

    displayStatus("Running " + algorithm + " algorithm.");
}

std::string GraphScene::getSceneTitle() const {
    return "Graph Visualization";
}