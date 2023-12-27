#pragma once
#include "template.hpp"
#include "monoid/monoid_reverse.hpp"

namespace Tree_Monoid_Base_Helper {
    template <typename M>
    struct MonoidX {
        using type = M;
    };
    template <typename M>
        requires requires { typename M::Monoid_A; }
    struct MonoidX<M> {
        using type = M::Monoid_X;
    };
    template <typename M>
    struct MonoidA {
        using type = M;
    };
    template <typename M>
        requires requires { typename M::Monoid_A; }
    struct MonoidA<M> {
        using type = M::Monoid_A;
    };
    template <template <typename> typename DS, typename M, bool>
    struct Base {};
    template <template <typename> typename DS, typename M>
    struct Base<DS, M, false> {
        mutable DS<Monoid_Reverse<M>> rds;
    };
    template <typename DS>
    concept is_dual = !requires (DS ds, int l, int r) { ds.prod(l, r); };
} // namespace Tree_Monoid_Base_Helper

template <template <typename> typename DS, typename TREE, typename Monoid, bool edge = false>
struct Tree_Monoid_Base: Tree_Monoid_Base_Helper::Base<DS, Monoid, Tree_Monoid_Base_Helper::MonoidX<Monoid>::type::commute> {
    using M = Monoid;
    using MX = Tree_Monoid_Base_Helper::MonoidX<M>::type;
    using MA = Tree_Monoid_Base_Helper::MonoidA<M>::type;
    using X = MX::value_type;
    using A = MA::value_type;

    mutable DS<M> ds;
    const TREE& tree;
    int n;
    Tree_Monoid_Base(const TREE& tree): tree(tree) { build(); }
    Tree_Monoid_Base(const TREE& tree, const Vec<X>& a): tree(tree) { build(a); }
    Tree_Monoid_Base(const TREE& tree, std::invocable<int> auto&& f): tree(tree) { build(f); }
    void build() {
        if constexpr (Tree_Monoid_Base_Helper::is_dual<DS<M>>)
            ds.build(n = tree.n);
        else
            build([&](int) { return MX::unit(); });
    }
    void build(const Vec<X>& a) {
        build([&](int i) { return a[i]; });
    }
    void build(std::invocable<int> auto&& f) {
        n = tree.n;
        if constexpr (edge) {
            auto fe = [&](int i) { return i ? f(tree.v_to_e(tree.id[i])) : MX::unit(); };
            ds.build(n, fe);
            if constexpr (!MX::commute)
                this->rds.build(n, fe);
        }
        else {
            auto fv = [&](int i) { return f(tree.id[i]); };
            ds.build(n, fv);
            if constexpr (!MX::commute)
                this->rds.build(n, fv);
        }
    }
    X get(int i) const {
        if constexpr (edge)
            i = tree.e_to_v(i);
        return ds.get(tree.lid[i]);
    }
    Vec<X> get_all() const {
        Vec<X> t = ds.get_all();
        if constexpr (edge) {
            Vec<X> res(n - 1);
            _for (i, n - 1)
                res[i] = t[tree.lid[tree.e_to_v(i)]];
            return res;
        }
        else {
            Vec<X> res(n);
            _for (i, n)
                res[i] = t[tree.lid[i]];
            return res;
        }
    }
    void set(int i, const X& x) {
        if constexpr (edge)
            i = tree.e_to_v(i);
        i = tree.lid[i];
        ds.set(i, x);
        if constexpr (!MX::commute)
            this->rds.set(i, x);
    }
    void multiply(int i, const X& x) {
        if constexpr (edge)
            i = tree.e_to_v(i);
        i = tree.lid[i];
        ds.multiply(i, x);
        if constexpr (!MX::commute)
            this->rds.multiply(i, x);
    }
    X prod_path(int u, int v) const {
        X r = MX::unit();
        for (auto&& [x, y]: tree.path_decomposition(u, v, edge))
            r = MX::op(r, prod(x, y));
        return r;
    }
    X prod_subtree(int u) const { return ds.prod(tree.lid[u] + edge, tree.rid[u]); }
    X prod_all() const { return prod_subtree(tree.id[0]); }
    void apply_path(int u, int v, const A& a) {
        for (auto&& [x, y]: tree.path_decomposition(u, v, edge))
            if (x <= y)
                ds.apply(x, y + 1, a);
            else
                ds.apply(y, x + 1, a);
    }
    void apply_subtree(int u, const A& a) { ds.apply(tree.lid[u] + edge, tree.rid[u], a); }
    void apply_outtree(int u, const A& a) {
        ds.apply(edge, tree.lid[u] + edge, a);
        ds.apply(tree.rid[u], n, a);
    }
    int max_path(auto&& check, int u, int v) {
        X r = MX::unit();
        auto check0 = [&](const X& x) { return check(MX::op(r, x)); };
        if constexpr (edge) {
            if (!check(MX::unit()))
                return -1;
            int lca = tree.lca(u, v);
            for (auto&& [x, y]: tree.path_decomposition(u, lca, edge)) {
                ASSERT(x >= y);
                X t = prod(x, y);
                if (check(MX::op(r, t))) {
                    r = MX::op(r, t);
                    u = tree.fa[tree.id[y]];
                }
                else {
                    int p;
                    if constexpr (MX::commute)
                        p = ds.min_left(check0, x + 1);
                    else
                        p = this->rds.min_left(check0, x + 1);
                    return p == x + 1 ? u : tree.fa[tree.id[p]];
                }
            }
            for (auto&& [x, y]: tree.path_decomposition(lca, v, edge)) {
                ASSERT(x <= y);
                X t = prod(x, y);
                if (check(MX::op(r, t))) {
                    r = MX::op(r, t);
                    v = tree.id[y];
                }
                else {
                    int p = ds.max_right(check0, x);
                    return p == x ? u : tree.id[p - 1];
                }
            }
        }
        else {
            if (!check(prod(tree.lid[u], tree.lid[u])))
                return -1;
            for (auto&& [x, y]: tree.path_decomposition(u, v, edge)) {
                X t = prod(x, y);
                if (check(MX::op(r, t))) {
                    r = MX::op(r, t);
                    u = tree.id[y];
                }
                else {
                    if (x <= y) {
                        int p = ds.max_right(check0, x);
                        return p == x ? u : tree.id[p - 1];
                    }
                    int p;
                    if constexpr (MX::commute)
                        p = ds.min_left(check0, x + 1);
                    else
                        p = this->rds.min_left(check0, x + 1);
                    return p == x + 1 ? u : tree.id[p];
                }
            }
        }
        return v;
    }

private:
    X prod(int x, int y) const {
        if constexpr (MX::commute)
            return x <= y ? ds.prod(x, y + 1) : ds.prod(y, x + 1);
        else
            return x <= y ? ds.prod(x, y + 1) : this->rds.prod(y, x + 1);
    }
};