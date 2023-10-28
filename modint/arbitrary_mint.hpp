#pragma once
#include "modint/barrett_reduction.hpp"

template <int id>
struct ArbitraryModIntBase {
    using Z = ArbitraryModIntBase;

    ArbitraryModIntBase() = default;

    ArbitraryModIntBase(i64 y) {
        int z = y % mod();
        if (z < 0)
            z += mod();
        x = z;
    }
    Z& operator+=(const Z& p) {
        x += p.x;
        if (x >= mod())
            x -= mod();
        return *this;
    }
    Z& operator-=(const Z& p) {
        x += mod() - p.x;
        if (x >= mod())
            x -= mod();
        return *this;
    }
    Z& operator*=(const Z& p) {
        x = rem((unsigned long long)x * p.x);
        return *this;
    }
    Z& operator/=(const Z& p) {
        return *this *= p.inv();
    }
    Z operator-() const { return Z(-x); }
    Z operator+() const { return *this; }
    friend Z operator+(Z lhs, const Z& rhs) {
        return lhs += rhs;
    }
    friend Z operator-(Z lhs, const Z& rhs) {
        return lhs -= rhs;
    }
    friend Z operator*(Z lhs, const Z& rhs) {
        return lhs *= rhs;
    }
    friend Z operator/(Z lhs, const Z& rhs) {
        return lhs /= rhs;
    }
    Z inv() const {
        int a = x, b = mod(), u = 1, v = 0, t;
        while (b > 0) {
            t = a / b;
            swap(a -= t * b, b);
            swap(u -= t * v, v);
        }
        return Z(u);
    }
    Z pow(i64 n) const {
        Z ret(1), mul(x);
        while (n > 0) {
            if (n & 1)
                ret *= mul;
            mul *= mul;
            n >>= 1;
        }
        return ret;
    }

    int val() const { return x; }
    static int& mod() {
        static int mod = 0;
        return mod;
    }
    static void set_mod(int p) {
        assert(0 < p && p <= (1 << 30) - 1);
        mod() = p;
        barrett() = Barrett(p);
    }

private:
    int x = 0;

    u32 rem(u64 p) { return barrett().rem(p); }
    static Barrett& barrett() {
        static Barrett b;
        return b;
    }
};

using ArbitraryModInt = ArbitraryModIntBase<-1>;