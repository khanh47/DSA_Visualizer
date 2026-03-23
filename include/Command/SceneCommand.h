#pragma once

#include "ICommand.h"
#include "SceneManager.h"
#include "BaseScene.h"
#include <memory>
#include <functional>
#include <string>

class SceneCommand : public ICommand {
public:
    using SceneFactory = std::function<std::unique_ptr<BaseScene>(SceneManager&)>;

    SceneCommand(SceneManager& manager, SceneFactory factory = nullptr, const std::string& commandName = "SceneCommand");
    void execute() override;

private:
    SceneManager& sceneManager;
    SceneFactory sceneFactory;
    std::string commandName;
};

// Factory function to create visualize scene command
inline std::unique_ptr<SceneCommand> createLinkedListSceneCommand(SceneManager& manager) {
    return std::unique_ptr<SceneCommand>(new SceneCommand(
        manager,
        [](SceneManager& mgr) -> std::unique_ptr<BaseScene> {
            extern std::unique_ptr<BaseScene> createLinkedListSceneFactory(SceneManager&);
            return createLinkedListSceneFactory(mgr);
        },
        "Push LinkedList Scene"
    ));
}

inline std::unique_ptr<SceneCommand> createHashTableSceneCommand(SceneManager& manager) {
    return std::unique_ptr<SceneCommand>(new SceneCommand(
        manager,
        [](SceneManager& mgr) -> std::unique_ptr<BaseScene> {
            extern std::unique_ptr<BaseScene> createHashTableSceneFactory(SceneManager&);
            return createHashTableSceneFactory(mgr);
        },
        "Push HashTable Scene"
    ));
}

inline std::unique_ptr<SceneCommand> createTreeSceneCommand(SceneManager& manager) {
    return std::unique_ptr<SceneCommand>(new SceneCommand(
        manager,
        [](SceneManager& mgr) -> std::unique_ptr<BaseScene> {
            extern std::unique_ptr<BaseScene> createTreeSceneFactory(SceneManager&);
            return createTreeSceneFactory(mgr);
        },
        "Push Tree Scene"
    ));
}

inline std::unique_ptr<SceneCommand> createGraphSceneCommand(SceneManager& manager) {
    return std::unique_ptr<SceneCommand>(new SceneCommand(
        manager,
        [](SceneManager& mgr) -> std::unique_ptr<BaseScene> {
            extern std::unique_ptr<BaseScene> createGraphSceneFactory(SceneManager&);
            return createGraphSceneFactory(mgr);
        },
        "Push Graph Scene"
    ));
}

// Factory function to create pop scene command
inline std::unique_ptr<SceneCommand> createPopSceneCommand(SceneManager& manager) {
    return std::unique_ptr<SceneCommand>(new SceneCommand(
        manager,
        nullptr,  // No factory needed for pop
        "Pop Scene"
    ));
}