#define PROBLEM "https://judge.yosupo.jp/problem/jump_on_tree"

#include "all.hpp"
#include "graph/tree.hpp"

int main() {
    dR(int, n, q);
    Graph<void> g(n);
    g.read_tree(0);
    Tree tree(g);
    _for (q) {
        dR(int, u, v, x);
        print(tree.jump(u, v, x));
    }
    return 0;
}