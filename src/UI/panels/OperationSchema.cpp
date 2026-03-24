#include "OperationSchema.h"

std::vector<OperationMenuItem> getOperationMenuItems(bool isGraphScene) {
    if (!isGraphScene) {
        return {
            {"Insert", OperationType::INSERT},
            {"Delete", OperationType::DELETE},
            {"Search", OperationType::SEARCH},
            {"Update", OperationType::UPDATE}
        };
    }

    return {
        {"Vertex", OperationType::VERTEX},
        {"Edge", OperationType::EDGE},
        {"Random", OperationType::RANDOM},
        {"Run", OperationType::RUN}
    };
}

OperationSchema getOperationSchema(OperationType type) {
    switch (type) {
        case OperationType::INSERT:
            return {{{"Insert Value", "Enter value to insert"}}, false, "", {}};
        case OperationType::SEARCH:
            return {{{"Search Value", "Enter value to search"}}, false, "", {}};
        case OperationType::DELETE:
            return {{{"Delete Value", "Enter value to delete"}}, false, "", {}};
        case OperationType::UPDATE:
            return {
                {
                    {"Update Key", "Enter key"},
                    {"Update Value", "Enter new value"}
                },
                false,
                "",
                {}
            };
        case OperationType::VERTEX:
            return {{{"Vertex Name", "Enter vertex name"}}, false, "", {}};
        case OperationType::EDGE:
            return {
                {
                    {"Vertex 1", "Enter vertex 1"},
                    {"Vertex 2", "Enter vertex 2"},
                    {"Cost", "Enter edge cost"}
                },
                false,
                "",
                {}
            };
        case OperationType::RANDOM:
            return {{{"Number of Nodes", "Enter number of nodes"}}, false, "", {}};
        case OperationType::RUN:
            return {{}, true, "Algorithm", {"Prim", "Kruskal"}};
    }

    return {{{"Value", "Enter value"}}, false, "", {}};
}
