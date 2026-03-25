#pragma once

#include "BaseVisualizer.h"
#include "KruskalDataStructure.h"
#include "VisualNode.h"

#include <memory>

class KruskalVisualizer : public BaseVisualizer {
public:
    KruskalVisualizer();
    ~KruskalVisualizer() override = default;

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
    KruskalDataStructure graph;
    std::unique_ptr<UI::VisualNode> demoNode;
    float playbackSpeed = 1.0f;
    bool autoRun = false;
};