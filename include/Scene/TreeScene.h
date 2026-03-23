#pragma once
#include "VisualizationScene.h"
#include <SFML/Graphics.hpp>

class TreeScene : public VisualizationScene {
public:
    TreeScene(SceneManager& sceneManager);
    virtual ~TreeScene() = default;

    // Hook methods from VisualizationScene
    void onInsert(const std::string& value) override;
    void onSearch(const std::string& value) override;
    void onDelete(const std::string& value) override;
    void onUpdate(const std::string& key, const std::string& value) override;

protected:
    std::string getSceneTitle() const override;
};
