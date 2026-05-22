#include <cassert>
#include <sstream>
#include <vector>

#include "gwen/container/mdarray.hpp"
#include "gwen/my_template.hpp"

using namespace gwen;

int main() {
    {
        mdarray<int, 2> a({2, 3}, -1);
        assert(a.shape[0] == 2 && a.shape[1] == 3);
        assert(a.size() == 6);
        a(0, 1) = 10;
        a(1, 2) = 20;
        assert(a.raw_data()[1] == 10);
        assert(a.raw_data()[5] == 20);
        assert(a(0, 1) == 10);
        assert(a(1, 2) == 20);
        assert(a.view()(0, 1) == 10);
    }

    {
        mdarray<int, 3> a({2, 2, 2});
        for (i32 i = 0; i < 2; ++i)
            for (i32 j = 0; j < 2; ++j)
                for (i32 k = 0; k < 2; ++k) a(i, j, k) = i * 100 + j * 10 + k;
        assert(a(1, 0, 1) == 101);
        assert(a.raw_data()[7] == 111);
    }

    {
        mdarray<int, 1> a(4);
        a(2) = 42;
        assert(a(2) == 42);
    }

    {
        const mdarray<int, 2> a({2, 2}, 7);
        assert(a(1, 0) == 7);
    }

    {
        mdarray<int, 2> a(2, 3);
        std::stringstream ss("10 20 30 40 50 60");
        ss >> a;
        assert(a(0, 0) == 10);
        assert(a(1, 2) == 60);
    }

    return 0;
}
