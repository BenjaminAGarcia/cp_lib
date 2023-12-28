#pragma once
#include "template.hpp"

template <typename Monoid>
struct Dual_FenwickTree {
    using M = Monoid;
    using A = M::value_type;

    int n;
    Vec<A> a;
    Dual_FenwickTree() = default;
    Dual_FenwickTree(int n) { build(n); }

    void build(int n) {
        this->n = n;
        a.assign(n, M::unit());
    }
    A get(int i) {
        A r = M::unit();
        for (i++; i <= n; i += lowbit(i))
            r = M::op(r, a[i - 1]);
        return r;
    }
    Vec<A> get_all() {
        Vec<A> a = this->a;
        _for_r (i, 1, n + 1) {
            int j = i + lowbit(i);
            if (j <= n)
                a[i - 1] = M::op(a[i - 1], a[j - 1]);
        }
        return a;
    }
    void apply(int l, int r, const A& x) {
        A inv = M::inverse(x);
        while (l < r) {
            a[r - 1] = M::op(x, a[r - 1]);
            r -= lowbit(r);
        }
        while (r < l) {
            a[l - 1] = M::op(inv, a[l - 1]);
            l -= lowbit(l);
        }
    }
};