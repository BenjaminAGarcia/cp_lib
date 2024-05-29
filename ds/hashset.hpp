#pragma once
#include "random/hash.hpp"

template <typename K>
struct HashSet {
    u32 cap, mask, shift;
    vc<K> key;
    vc<bool> vis;
    HashSet(int n = 0) { build(n); }
    void reserve(int n) {
        if (n > len(key)) {
            HashSet<K> hs(n);
            _for (i, len(key))
                if (vis[i])
                    hs.insert(key[i]);
            *this = std::move(hs);
        }
    }
    bool insert(K k) {
        if (!cap)
            extend();
        int i = index(k);
        if (!vis[i]) {
            vis[i] = true;
            key[i] = std::move(k);
            cap--;
            return true;
        }
        return false;
    }
    bool contains(const K& k) const {
        int i = index(k);
        return vis[i];
    }
    void clear() { std::fill(all(vis), false), cap = len(key) / 2; }
    int size() const { return len(key) / 2 - cap; }
    void enumerate(auto&& f) const {
        _for (i, len(vis))
            if (vis[i])
                f(key[i]);
    }
    vc<K> items() const {
        vc<K> res;
        enumerate([&](const K& k) { res.eb(k); });
        return res;
    }

private:
    void build(int n) {
        int k = 8;
        while (k < n * 2)
            k <<= 1;
        cap = k / 2;
        mask = k - 1;
        shift = 64 - std::__lg(k);
        key.resize(k);
        vis.assign(k, false);
    }
    void extend() { reserve(len(key) * 2); }
    int index(const K& k) const {
        int i = hash(k) >> shift;
        while (vis[i] && key[i] != k)
            i = (i + 1) & mask;
        return i;
    }
};