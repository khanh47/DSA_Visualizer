#include "OperationCommand.h"
#include "OperationPanel.h"

OperationCommand::OperationCommand(OperationPanel* panel, OperationType type)
    : panel(panel), type(type) {}

void OperationCommand::execute() {
    if (!panel) return;
    panel->get(type);    
}
