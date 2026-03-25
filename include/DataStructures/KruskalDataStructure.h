#pragma once

#include <string>
#include <vector>

class KruskalDataStructure {
public:
    struct Edge {
        int u = 0;
        int v = 0;
        int w = 0;
    };

    KruskalDataStructure() = default;
    ~KruskalDataStructure() = default;

    void clear();
    void setVertexCount(int n);
    void addEdge(int u, int v, int w);
    std::vector<Edge> runKruskal() const;

    int getVertexCount() const;
    const std::vector<Edge>& getEdges() const;

private:
    int vertexCount = 0;
    std::vector<Edge> edges;
};