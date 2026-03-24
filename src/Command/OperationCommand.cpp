#include "OperationCommand.h"
#include "OperationPanel.h"
#include "VisualizationScene.h"

OperationCommand::OperationCommand(OperationPanel* panel, OperationType type)
    : panel(panel), type(type) {}

void OperationCommand::execute() {
    if (!panel) return;
    panel->get(type);
}

RunCommand::RunCommand(VisualizationScene* scene, OperationPanel* panel, OperationType type)
    : scene(scene), panel(panel), type(type) {}

void RunCommand::execute() {
    if (!scene) {
        return;
    }

    const std::string value = panel ? panel->getInputValue(0) : "";

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
        case OperationType::UPDATE: {
            const std::string newValue = panel ? panel->getInputValue(1) : "";
            scene->onUpdate(value, newValue);
            break;
        }
        case OperationType::RANDOM:
            scene->onRandom(value);
            break;
        case OperationType::RUN: {
            const std::string algorithm = panel ? panel->getRunAlgorithm() : "prim";
            scene->onRun(algorithm);
            break;
        }
        case OperationType::VERTEX:
            scene->onVertex(value);
            break;
        case OperationType::EDGE: {
            const std::string vertex2 = panel ? panel->getInputValue(1) : "";
            const std::string cost = panel ? panel->getInputValue(2) : "";
            scene->onEdge(value, vertex2, cost);
            break;
        }
    }
}
