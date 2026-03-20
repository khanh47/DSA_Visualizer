#include "OperationCommand.h"
#include "VisualizationScene.h"

OperationCommand::OperationCommand(VisualizationScene* scene, OperationType type, const std::string& value)
    : scene(scene), type(type), value(value) {}

void OperationCommand::execute() {
    if (!scene) return;
    
    switch (type) {
        case OperationType::INSERT:
            scene->onInsert(value);
            break;
        case OperationType::SEARCH:
            scene->onSearch(value);
            break;
        case OperationType::DELETE:
            scene->onDelete(value);
            break;
    }
}
