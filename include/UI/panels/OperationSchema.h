#pragma once

#include <string>
#include <vector>
#include "OperationCommand.h"

struct InputFieldDef {
    std::string label;
    std::string placeholder;
};

struct OperationMenuItem {
    std::string label;
    OperationType type;
};

struct OperationSchema {
    std::vector<InputFieldDef> inputFields;
    bool useSelector = false;
    std::string selectorLabel;
    std::vector<std::string> selectorOptions;
};

std::vector<OperationMenuItem> getOperationMenuItems(bool isGraphScene);
OperationSchema getOperationSchema(OperationType type);
