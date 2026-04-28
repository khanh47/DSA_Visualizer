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
            // Single-parameter version for LinkedList, Trie, Kruskal
            // Two-parameter version for HashTable (Key-Value pairs)
            if (isHT)
                scene->onInsert(menu->getInputValue(0), menu->getInputValue(1)); // (Key, Value)
            else
                scene->onInsert(menu->getInputValue(0)); // (Value) only
            break;

        case OperationType::DELETE:
            // LinkedList/Trie: Input[1] = Index/Word | HashTable: Input[2] = Key
            scene->onDelete(menu->getInputValue(isHT ? 2 : 1));
            break;

        case OperationType::SEARCH:
            // LinkedList/Trie: Input[2] = Value/Word | HashTable: Input[3] = Key
            scene->onSearch(menu->getInputValue(isHT ? 3 : 2));
            break;

        case OperationType::UPDATE:
            // LinkedList/Trie: Input[3]=Index, Input[4]=NewValue | HashTable: Input[4]=Key, Input[5]=NewValue
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
