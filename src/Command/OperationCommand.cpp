#include "OperationCommand.h"
#include "OperationMenu.h"
#include "VisualizationScene.h"

OperationCommand::OperationCommand(VisualizationScene* scene, OperationMenu* menu, OperationType type)
    : scene(scene), menu(menu), type(type) {}

void OperationCommand::execute() {
    if (!scene) {
        return;
    }

    bool isHT = scene->getSceneTitle() == "Hash Table Visualization";
    
    switch (type) {
        case OperationType::INSERT:
            if (isHT)
                scene->onInsert(menu->getInputValue(0), menu->getInputValue(1)); // Key-Value
            else
                scene->onInsert(menu->getInputValue(0), ""); // Single Value
            break;

        case OperationType::DELETE:
            // Nếu là HT, nút Insert chiếm 2 ô (0,1), nên ô Delete là ô số 2.
            // Nếu không, nút Insert chiếm 1 ô (0), nên ô Delete là ô số 1.
            scene->onDelete(menu->getInputValue(isHT ? 2 : 1));
            break;

        case OperationType::SEARCH:
            // Tương tự: HT là ô số 3, còn lại là ô số 2.
            scene->onSearch(menu->getInputValue(isHT ? 3 : 2));
            break;

        case OperationType::UPDATE:
            // HT: ô 4 (Key), ô 5 (Value).
            // Còn lại: ô 3 (Index/Old), ô 4 (New).
            if (isHT)
                scene->onUpdate(menu->getInputValue(4), menu->getInputValue(5));
            else
                scene->onUpdate(menu->getInputValue(3), menu->getInputValue(4));
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
