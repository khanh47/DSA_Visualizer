#include "OperationCommand.h"
#include "OperationMenu.h"
#include "VisualizationScene.h"

OperationCommand::OperationCommand(VisualizationScene* scene, OperationMenu* menu, OperationType type)
    : scene(scene), menu(menu), type(type) {}

void OperationCommand::execute() {
    if (!scene) {
        return;
    }

    switch (type) {
        case OperationType::INSERT:
            scene->onInsert(menu ? menu->getInputValue(0) : "");
            break;
        case OperationType::DELETE:
            scene->onDelete(menu ? menu->getInputValue(1) : "");
            break;
        case OperationType::SEARCH:
            scene->onSearch(menu ? menu->getInputValue(2) : "");
            break;
        case OperationType::UPDATE:
            scene->onUpdate(
                menu ? menu->getInputValue(3) : "",
                menu ? menu->getInputValue(4) : ""
            );
            break;
        case OperationType::RANDOM:
            scene->onRandom();
            break;
        case OperationType::RUN:
            scene->onRun();
            break;
        case OperationType::RESET:
            scene->onReset();
            break;
    }
}
