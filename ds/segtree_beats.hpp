#pragma once
#include "ds/lazy_segtree.hpp"

template <typename ActedMonoid>
using SegTree_Beats = Lazy_SegTree<ActedMonoid, true>;