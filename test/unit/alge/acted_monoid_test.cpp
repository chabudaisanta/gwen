// clang-format off
#include "gwen/alge/acted_monoid.hpp"
#include "gwen/alge/range_affine_range_sum_monoid.hpp"
#include "gwen/alge/monoid.hpp"
// clang-format on

#include <gtest/gtest.h>

#include "gwen/mod/modint.hpp"
#include "gwen/types.hpp"

using namespace gwen;
using mint = DynamicModInt64;
using AffineMonoid = range_affine_range_sum_monoid<mint>;

TEST(ActedMonoidTest, ConceptCheck) {
    // range_affine_range_sum_monoid should satisfy acted_monoid
    static_assert(acted_monoid<AffineMonoid>);

    // no_op_monoid should satisfy acted_monoid
    static_assert(acted_monoid<no_op_monoid<i32>>);

    // sum_monoid should NOT satisfy acted_monoid (it lacks F)
    static_assert(!acted_monoid<sum_monoid<i32>>);
}
