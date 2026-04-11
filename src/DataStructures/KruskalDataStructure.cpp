#include "KruskalDataStructure.h"

void KruskalDataStructure::clear() {
    numNodes = 0;
    edges.clear();
}

void KruskalDataStructure::addEdge(int u, int v, int w) {
    edges.push_back({u, v, w});
}

int KruskalDataStructure::randomValue(int l, int r) {
    if (l > r) std::swap(l, r);
    return l + (rd() % (r - l + 1));
}

void KruskalDataStructure::randomGraph() {
    edges.clear();
    numNodes = randomValue(0, 3) + 3;
    for (int i = 1; i < numNodes; ++i) {
        int p = randomValue(0, i - 1);
        addEdge(p, i, randomValue(-20, 20));
    }
    int numEdges = randomValue(0, numNodes - 2);
    while (numEdges--) {
        while (1) {
            int u = randomValue(0, numNodes - 1);
            int v = randomValue(0, numNodes - 1);
            if (u == v) continue;
            if (u > v) std::swap(u, v);
            bool isGraph = false;
            for (Edge edge : edges) {
                if (edge.u == u && edge.v == v) {
                    isGraph = true;
                    break;
                }
            } 
            if (!isGraph) {
                addEdge(u, v, randomValue(-20, 20));
                break;
            }
        }
    }
}

std::vector<KruskalDataStructure::Edge> KruskalDataStructure::runKruskal() const {
    DSU dsu;
    dsu.init(numNodes);
    std::vector<KruskalDataStructure::Edge> res;
    for (const Edge& edge : edges) {
        if (dsu.join(edge.u, edge.v)) {
            res.push_back(edge);
        }
    }
    return res;
}

int KruskalDataStructure::getNumNodes() const {
    return numNodes;
}

const std::vector<KruskalDataStructure::Edge>& KruskalDataStructure::getEdges() const {
    return edges;
}
