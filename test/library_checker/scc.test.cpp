#define PROBLEM "https://judge.yosupo.jp/problem/scc"

#include "all.hpp"
#include "graph/strongly_connected_component.hpp"

int main() {
    dR(int, n, m);
    Graph<bool, true> g(n);
    g.read_graph(m, false, 0);
    auto [scc, comp] = strongly_connected_component(g);
    vvi ans(scc);
    _for (i, n)
        ans[comp[i]].eb(i);
    writeln(len(ans));
    foreach (x, ans)
        writeln(len(x), ' ', x);
    return 0;
}