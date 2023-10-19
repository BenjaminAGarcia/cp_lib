#define PROBLEM "https://judge.yosupo.jp/problem/tree_diameter"

#include "all.hpp"
#include "graph/utility.hpp"

int main() {
    dR(int, n);
    Graph<int> g(n);
    g.read_tree( true, 0);
    auto [d, path] = diameter<i64>(g);
    writeln(d, ' ', len(path));
    writeln(path);
    return 0;
}