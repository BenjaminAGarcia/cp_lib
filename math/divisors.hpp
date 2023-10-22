#pragma once
#include "factorize.hpp"

template <Unsigned T>
Vec<T> divisors(T n) {
    Vec<T> d{1};
    for (auto&& [p, c]: factorize_pair(n)) {
        int t = len(d);
        T pp = 1;
        _for (c) {
            pp *= p;
            _for (i, t)
                d.eb(d[i] * pp);
        }
    }
    return d;
}