#define PROBLEM "https://judge.yosupo.jp/problem/biconnected_components"

#include "all.hpp"
#include "graph/block_cut_tree.hpp"

int main() {
    dR(int, n, m);
    Graph<bool> g(n);
    g.read_graph(m, false, 0);

    auto bct = block_cut_tree(g);
    writeln(bct.n - n);
    _for (i, n, bct.n) {
        vi ans;
        foreach (j, bct[i])
            if (j < n)
                ans.eb(j);
        writeln(len(ans), ' ', ans);
    }
    return 0;
}