#pragma once
#include "ICommand.h"
#include <memory>
#include <string>

enum class OperationType {
    INSERT,
    SEARCH,
    DELETE,
    UPDATE
};

class OperationPanel; // Forward declaration

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