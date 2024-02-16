#pragma once
#include "template.hpp"

template <typename Monoid>
struct Range_Assignment_SegTree {
    using M = Monoid;
    using X = M::value_type;
    int n, lg, sz;
    vc<X> a, cache;
    vi lazy;

    Range_Assignment_SegTree() = default;
    Range_Assignment_SegTree(int n) { build(n); }
    template <std::convertible_to<X> T>
    Range_Assignment_SegTree(const vc<T>& a) { build(a); }
    Range_Assignment_SegTree(int n, std::invocable<int> auto&& f) { build(n, f); }

    void build(int n) {
        build(n, [&](int i) { return M::unit(); });
    }
    template <std::convertible_to<X> T>
    void build(const vc<T>& a) {
        build(len(a), [&](int i) { return a[i]; });
    }
    void build(int n, std::invocable<int> auto&& f) {
        this->n = n;
        lg = get_lg(n);
        sz = 1 << lg;
        a.assign(sz << 1, M::unit());
        lazy.assign(sz, -1);
        _for (i, n)
            a[sz + i] = f(i);
        _for_r (i, 1, sz)
            update(i);
    }
    void update(int i) { a[i] = M::op(a[i << 1], a[i << 1 | 1]); }
    void set(int i, const X& x) {
        i += sz;
        _for_r (j, 1, lg + 1)
            push(i >> j);
        a[i] = x;
        _for (j, 1, lg + 1)
            update(i >> j);
    }
    void multiply(int i, const X& x) {
        i += sz;
        _for_r (j, 1, lg + 1)
            push(i >> j);
        a[i] = M::op(a[i], x);
        _for (j, 1, lg + 1)
            update(i >> j);
    }
    X get(int i) {
        i += sz;
        _for_r (j, 1, lg + 1)
            push(i >> j);
        return a[i];
    }
    vc<X> get_all() {
        _for (i, sz)
            push(i);
        return {a.begin() + sz, a.begin() + sz + n};
    }
    X prod(int l, int r) {
        if (l == r)
            return M::unit();
        l += sz, r += sz;
        _for_r (i, 1, lg + 1) {
            if (((l >> i) << i) != l)
                push(l >> i);
            if (((r >> i) << i) != r)
                push((r - 1) >> i);
        }
        X xl = M::unit(), xr = M::unit();
        while (l < r) {
            if (l & 1)
                xl = M::op(xl, a[l++]);
            if (r & 1)
                xr = M::op(a[--r], xr);
            l >>= 1, r >>= 1;
        }
        return M::op(xl, xr);
    }
    X prod_all() { return a[1]; }
    void apply(int l, int r, const X& x) {
        if (l == r)
            return;
        l += sz, r += sz;
        _for_r (i, 1, lg + 1) {
            if (((l >> i) << i) != l)
                push(l >> i);
            if (((r >> i) << i) != r)
                push((r - 1) >> i);
        }
        int l2 = l, r2 = r;
        int cur = pow(x);
        while (l < r) {
            if (l & 1)
                apply(l++, cur);
            if (r & 1)
                apply(--r, cur);
            l >>= 1, r >>= 1;
            cur++;
        }
        l = l2, r = r2;
        _for (i, 1, lg + 1) {
            if (((l >> i) << i) != l)
                update(l >> i);
            if (((r >> i) << i) != r)
                update((r - 1) >> i);
        }
    }
    int max_right(auto&& check, int l) {
        ASSERT(0 <= l && l <= n && check(M::unit()));
        if (l == n)
            return n;
        l += sz;
        _for_r (i, 1, lg + 1)
            push(l >> i);
        X t = M::unit();
        do {
            l >>= __builtin_ctz(l);
            if (!check(M::op(t, a[l]))) {
                while (l < sz) {
                    push(l);
                    l <<= 1;
                    if (check(M::op(t, a[l])))
                        t = M::op(t, a[l++]);
                }
                return l - sz;
            }
            t = M::op(t, a[l++]);
        } while (l & (l - 1));
        return n;
    }
    int min_left(auto&& check, int r) {
        ASSERT(0 <= r && r <= n && check(M::unit()));
        if (r == 0)
            return 0;
        r += sz;
        _for_r (i, 1, lg + 1)
            push((r - 1) >> i);
        r--;
        X t = M::unit();
        do {
            r >>= __builtin_ctz(~r);
            if (!check(M::op(a[r], t))) {
                while (r < sz) {
                    push(r);
                    r = r << 1 | 1;
                    if (check(M::op(a[r], t)))
                        t = M::op(a[r--], t);
                }
                return r + 1 - sz;
            }
            t = M::op(a[r--], t);
        } while (r & (r + 1));
        return 0;
    }

private:
    void apply(int i, int x) {
        a[i] = cache[x];
        if (i < sz) {
            lazy[i] = x;
        }
    }
    void push(int i) {
        if (lazy[i] == -1)
            return;
        apply(i << 1, lazy[i] - 1);
        apply(i << 1 | 1, lazy[i] - 1);
        lazy[i] = -1;
    }
    int pow(X x) {
        int r = len(cache);
        _for (i, lg + 1) {
            cache.eb(x);
            x = M::op(x, x);
        }
        return r;
    }
};
template <typename ActedMonoid>
using Lazy_SegTree = Range_Assignment_SegTree<ActedMonoid>;