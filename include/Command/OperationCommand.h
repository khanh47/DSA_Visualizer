#pragma once
#include "ICommand.h"
#include <memory>
#include <string>

enum class OperationType {
    INSERT,
    DELETE,
    SEARCH,
    UPDATE,
    RANDOM,
    RUN,
    RESET
};

class VisualizationScene; // Forward declaration
class OperationMenu; // Forward declaration

class OperationCommand : public ICommand {
public:
    OperationCommand(VisualizationScene* scene, OperationMenu* menu, OperationType type);
    
    void execute() override;

private:
    VisualizationScene* scene;
    OperationMenu* menu;
    OperationType type;
};

inline std::unique_ptr<ICommand> createOperationCommand(VisualizationScene* scene, OperationMenu* menu, OperationType type) {
    return std::unique_ptr<ICommand>(new OperationCommand(scene, menu, type));
}