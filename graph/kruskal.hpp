#pragma once
#include "base.hpp"
#include "../ds/dsu.hpp"

template <typename T>
struct KruskalResult {
    T cost;
    Vec<bool> in;
    Graph<T> mst;
};
template <typename T>
auto kruskal(Graph<T>& g) {
    int n = g.n, m = g.m;
    Vec<std::pair<T, int>> edges;
    _for (i, m)
        edges.eb(g.edges[i].cost, i);
    sort(edges);
    DSU dsu(n);
    T cost{};
    Vec<bool> in(m);
    Graph<T> mst(n, n - 1);
    for (auto&& [_, i]: edges)
        if (auto&& e = g.edges[i]; dsu.merge(e.from, e.to)) {
            in[i] = 1;
            mst.add(e.from, e.to, e.cost);
            cost += e.cost;
        }
    mst.build();
    return KruskalResult{cost, std::move(in), std::move(mst)};
}