#define PROBLEM "https://judge.yosupo.jp/problem/cycle_detection_undirected"

#include "all.hpp"
#include "graph/find_cycle.hpp"

int main() {
    dR(int, n, m);
    Graph<int> g(n);
    g.read_graph(m, false, 0);
    auto [vs, es] = find_cycle(g);
    if (vs.empty()) {
        writeln(-1);
    }
    else {
        writeln(len(vs));
        writeln(vs);
        writeln(es);
    }
    return 0;
}