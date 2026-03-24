#pragma once
#include "BaseScene.h"
#include "Button.h"
#include "BaseVisualizer.h"
#include "SceneManager.h"
#include "OperationPanel.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class VisualizationScene : public BaseScene {
public:
    VisualizationScene(SceneManager& manager);
    virtual ~VisualizationScene() = default;

    // Public methods for operation dispatch
    void setVisualizer(BaseVisualizer* visualizer);
    void displayStatus(const std::string& message);

    // BaseScene overrides
    void processEvents(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    // Generic hooks with default no-op behavior.
    virtual void onInsert(const std::string& value) { (void)value; }
    virtual void onSearch(const std::string& value) { (void)value; }
    virtual void onDelete(const std::string& value) { (void)value; }
    virtual void onUpdate(const std::string& key, const std::string& value) {
        (void)key;
        (void)value;
    }

    // Graph-specific hooks with default no-op behavior.
    virtual void onRandom(const std::string& value) { (void)value; }
    virtual void onRun(const std::string& algorithm) { (void)algorithm; }
    virtual void onVertex(const std::string& value) { (void)value; }
    virtual void onEdge(const std::string& from, const std::string& to, const std::string& cost) {
        (void)from;
        (void)to;
        (void)cost;
    }

    // Helper method for child classes to override
    virtual std::string getSceneTitle() const = 0;

protected:
    void initializeOperationPanel();

    SceneManager& manager;
    sf::Text statusText;
    std::unique_ptr<UI::Button> backButton;
    std::unique_ptr<OperationPanel> operationPanel;
};