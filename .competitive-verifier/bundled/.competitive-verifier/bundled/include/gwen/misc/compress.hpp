#line 1 ".competitive-verifier/bundled/include/gwen/misc/compress.hpp"
#line 2 "include/gwen/misc/compress.hpp"
#include <algorithm>
#include <vector>
namespace gwen {
template <typename T> std::vector<int> compress(const std::vector<T>& vec) {
    std::vector<T> copy = vec;
    std::sort(copy.begin(), copy.end());
    copy.erase(std::unique(copy.begin(), copy.end()), copy.end());
    std::vector<int> ret(vec.size());
    for (int i = 0; const T& x : vec) {
        ret[i] = std::lower_bound(copy.begin(), copy.end(), x) - copy.begin();
        ++i;
    }
    return ret;
}

}  // namespace gwen
