# Graham Scan（凸包）

与えられた点集合の凸包を求める関数です。戻り値は凸包上の点の列（反時計回り）です。

---

## 関数

```
template <typename T, bool contain_on_boundary = false>
vector<point<T>> graham_scan(vector<point<T>> points);
```

- **入力**: 2 次元の点の列 `points`。
- **出力**: 凸包を成す点の列。反時計回りに並び、最も下（$y$ 最小、同率なら $x$ 最小）の点から始まります。
- **テンプレート**:
  - `T`: 座標の型（例: `i64`, `double`）。
  - `contain_on_boundary`: `true` のとき、凸包の辺上にある点も含めます。`false` のときは頂点のみです。

**制約**

- 点の数が 2 以下のときは、そのまま（または 2 点）を返します。

**計算量**

- $O(n \log n)$（ソートがボトルネック）

---

## 使用例

```cpp
#include "gwen/geo/graham_scan.hpp"
#include "gwen/geo/point.hpp"

using namespace gwen;

vector<point<i64>> ps = {{0, 0}, {1, 0}, {1, 1}, {0, 1}, {1, 2}};
auto hull = graham_scan<i64>(ps);           // 頂点のみ
auto hull2 = graham_scan<i64, true>(ps);    // 辺上の点も含む
```
