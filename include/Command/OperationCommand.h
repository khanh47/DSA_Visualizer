#pragma once
#include "ICommand.h"
#include <memory>
#include <string>

enum class OperationType {
    INSERT,
    SEARCH,
    DELETE,
    UPDATE,
    RANDOM,
    RUN,
    VERTEX,
    EDGE
};

class OperationPanel; // Forward declaration
class VisualizationScene; // Forward declaration

class OperationCommand : public ICommand {
public:
    OperationCommand(OperationPanel* panel, OperationType type);
    
    void execute() override;

private:
    OperationPanel* panel;
    OperationType type;
};

inline std::unique_ptr<ICommand> createOperationCommand(OperationPanel* scene, OperationType type) {
    return std::unique_ptr<ICommand>(new OperationCommand(scene, type));
}

inline std::unique_ptr<ICommand> createInsertCommand(OperationPanel* scene) {
    return createOperationCommand(scene, OperationType::INSERT);
}

inline std::unique_ptr<ICommand> createSearchCommand(OperationPanel* scene) {
    return createOperationCommand(scene, OperationType::SEARCH);
}

inline std::unique_ptr<ICommand> createDeleteCommand(OperationPanel* scene) {
    return createOperationCommand(scene, OperationType::DELETE);
}

inline std::unique_ptr<ICommand> createUpdateCommand(OperationPanel* scene) {
    return createOperationCommand(scene, OperationType::UPDATE);
}

inline std::unique_ptr<ICommand> createRandomCommand(OperationPanel* scene) {
    return createOperationCommand(scene, OperationType::RANDOM);
}

inline std::unique_ptr<ICommand> createRunModeCommand(OperationPanel* scene) {
    return createOperationCommand(scene, OperationType::RUN);
}

inline std::unique_ptr<ICommand> createVertexCommand(OperationPanel* scene) {
    return createOperationCommand(scene, OperationType::VERTEX);
}

inline std::unique_ptr<ICommand> createEdgeCommand(OperationPanel* scene) {
    return createOperationCommand(scene, OperationType::EDGE);
}

class RunCommand : public ICommand {
public:
    RunCommand(VisualizationScene* scene, OperationPanel* panel, OperationType type);
    
    void execute() override;

private:
    VisualizationScene* scene;
    OperationPanel* panel;
    OperationType type;
};

inline std::unique_ptr<ICommand> createRunCommand(VisualizationScene* scene, OperationPanel* panel, OperationType type) {
    return std::unique_ptr<ICommand>(new RunCommand(scene, panel, type));
}