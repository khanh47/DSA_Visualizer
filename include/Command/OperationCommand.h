#pragma once
#include "ICommand.h"
#include <string>

enum class OperationType {
    INSERT,
    SEARCH,
    DELETE,
    UPDATE
};

class VisualizationScene; // Forward declaration

class OperationCommand : public ICommand {
public:
    OperationCommand(VisualizationScene* scene, OperationType type, const std::string& value);
    
    void execute() override;

private:
    VisualizationScene* scene;
    OperationType type;
    std::string value;
};