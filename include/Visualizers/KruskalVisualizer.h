#pragma once

#include "BaseVisualizer.h"
#include "KruskalDataStructure.h"
#include "VisualEdge.h"
#include "VisualNode.h"

#include <memory>
#include <vector>

class KruskalVisualizer : public BaseVisualizer {
public:
    KruskalVisualizer();
    ~KruskalVisualizer() override = default;

    void random();
    void run();

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

    KruskalDataStructure& data();
    const KruskalDataStructure& data() const;

private:
    void buildStaticLayout();
    void syncAlgorithmState();

    KruskalDataStructure graph;
    std::vector<KruskalDataStructure::Edge> mstEdges;
    std::vector<sf::Vector2f> layoutPositions;
    std::vector<int> currentParents;
    sf::Font* font = nullptr;
    std::size_t currentEdgeIndex = 0;
    std::size_t currentMstEdge = 0;
    float animationElapsed = 0.0f;
    float playbackSpeed = 1.0f;
    bool autoRun = true;
    bool animationReady = false;
};