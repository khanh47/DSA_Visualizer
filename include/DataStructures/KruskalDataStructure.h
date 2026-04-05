#pragma once

#include <algorithm>
#include <chrono>
#include <random>
#include <string>
#include <vector>

class KruskalDataStructure {
public:
    struct Edge {
        int u = 0;
        int v = 0;
        int w = 0;
    };

    struct DSU {
        std::vector<int> lab;

        void init(int n) { lab.resize(n, -1); }

        int Find(int u) { return lab[u] < 0 ? u : lab[u] = Find(lab[u]); }

        bool join(int u, int v) {
            u = Find(u); v = Find(v);
            if (u == v) return false;
            if (lab[u] > lab[v]) std::swap(u, v);
            lab[u] += lab[v];
            lab[v] = u;
            return true;
        }
    };

    bool cmp(Edge a, Edge b) {
        return a.w < b.w;
    };

    KruskalDataStructure() = default;
    ~KruskalDataStructure() = default;

    std::mt19937 rd{static_cast<unsigned int>(
        std::chrono::steady_clock::now().time_since_epoch().count())};

    void clear();
    void addEdge(int u, int v, int w);
    void randomGraph();
    int randomValue(int l, int r);
    std::vector<Edge> runKruskal() const;

    int getNumNodes() const;
    const std::vector<Edge>& getEdges() const;

private:
    int numNodes = 0;
    std::vector<Edge> edges;
};