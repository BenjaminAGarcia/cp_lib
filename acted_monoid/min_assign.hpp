#pragma once
#include "monoid/min.hpp"
#include "monoid/assign.hpp"

template <typename T, T none = inf<T>>
struct ActedMonoid_Min_Assign {
    using Monoid_X = Monoid_Min<T>;
    using Monoid_A = Monoid_Assign<T, none>;
    using X = Monoid_X::value_type;
    using A = Monoid_A::value_type;
    static constexpr X act(const X& x, const A& a, int) {
        return a == none ? x : a;
    }
};