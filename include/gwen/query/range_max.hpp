#pragma once

#include "gwen/alge/monoid.hpp"
#include "gwen/ds/segment_tree.hpp"

namespace gwen {

/**
 * @brief 点更新・区間最大値を扱うセグメント木の型エイリアス
 * @tparam T 要素型。`max_monoid<T>` の単位元が有効であること
 */
template <typename T> using RangeMax = SegmentTree<max_monoid<T>>;

}  // namespace gwen
