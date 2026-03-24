#pragma once
#include "VisualizationScene.h"
#include <SFML/Graphics.hpp>

class GraphScene : public VisualizationScene {
public:
    GraphScene(SceneManager& sceneManager);
    virtual ~GraphScene() = default;

    void onRandom(const std::string& value) override;
    void onRun(const std::string& algorithm) override;
    void onVertex(const std::string& value) override;
    void onEdge(const std::string& from, const std::string& to, const std::string& cost) override;

protected:
    std::string getSceneTitle() const override;
};
