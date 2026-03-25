#include "KruskalDataStructure.h"

void KruskalDataStructure::clear() {
    vertexCount = 0;
    edges.clear();
}

void KruskalDataStructure::setVertexCount(int n) {
    vertexCount = (n < 0) ? 0 : n;
}

void KruskalDataStructure::addEdge(int u, int v, int w) {
    edges.push_back({u, v, w});
}

std::vector<KruskalDataStructure::Edge> KruskalDataStructure::runKruskal() const {
    // Skeleton: real MST logic will be implemented later.
    return {};
}

int KruskalDataStructure::getVertexCount() const {
    return vertexCount;
}

const std::vector<KruskalDataStructure::Edge>& KruskalDataStructure::getEdges() const {
    return edges;
}
