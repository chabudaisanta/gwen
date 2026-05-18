#include <cassert>
#include <string>
#include <vector>

#include "gwen/algebra/range_affine_range_sum_monoid.hpp"
#include "gwen/container/implicit_treap.hpp"

using namespace gwen;

struct concat_monoid {
    using S = std::string;
    static S op(const S& a, const S& b) { return a + b; }
    static S e() { return ""; }
    struct F {};
    static S mapping(F, const S& x) { return x; }
    static F composition(F, F) { return {}; }
    static F id() { return {}; }
};

int main() {
    // non-commutative: reverse + prod
    {
        implicit_treap<concat_monoid> t(std::vector<std::string>{"a", "b", "c", "d"});
        assert(t.prod(0, 4) == "abcd");
        t.reverse(1, 3);  // ab -> ba => "abacd" wait [1,3) is indices 1,2 => "bc" -> "cb" => "acbd"
        assert(t.prod(0, 4) == "acbd");
        t.reverse(0, 4);
        assert(t.prod(0, 4) == "dbca");
        assert(t.to_vec() == std::vector<std::string>({"d", "b", "c", "a"}));
    }

    // range affine + sum (regression)
    {
        using Monoid = range_affine_range_sum_monoid<long long>;
        implicit_treap<Monoid> t;
        for (long long x : {1, 2, 3, 4}) t.push_back(Monoid::unit(x));
        assert(t.prod(1, 3).val == 5);
        t.apply(0, 4, Monoid::F{2, 1});
        assert(t.prod(0, 4).val == 2 * (1 + 2 + 3 + 4) + 4);
        t.reverse(0, 2);
        assert(t.prod(0, 2).val == t.get(0).val + t.get(1).val);
    }

    return 0;
}
