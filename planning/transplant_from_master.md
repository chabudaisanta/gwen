
# 移植作業をしなければいけないヘッダー一覧

**このファイルにあるヘッダーのディレクトリ構成はmasterブランチのものです。必要に応じて変更すべきです。**
使用頻度が高いヘッダー -> 使用頻度がまあまあなヘッダー -> 使用頻度が低いけどライブラリとしてあった方がいいヘッダー -> クオリティが低いので1から作り直すべきヘッダー の順で作業したい。
本当に要らないヘッダーはここにリストアップしていないため、安心してほしい。

## 使用頻度が高いヘッダー
- algebra/range_affine_range_sum_monoid.hpp
- algebra/rolling_hash_monoid.hpp
- container/wrapper/range_add_update_sum.hpp
- container/implicit_treap.hpp
- container/treap.hpp
- container/sorted_treap.hpp
- graph/csr.hpp
- graph/dijkstra.hpp
- graph/hld.hpp
- graph/graph.hpp
- hash/rolling_hash.hpp
- math/combination.hpp
- math/prime.hpp
- misc/compress.hpp
- misc/node_pool.hpp (treap系で使用)
- misc/xorshift.hpp
- mod/mod61.hpp (ロリハ用)
- mod/modint.hpp (dynamic_modintがprimeの中のmillerに使われてる)

## 使用頻度がまあまあなヘッダー
- container/implicit_treap.hpp
- container/bit_vector.hpp
- graph/weighted_dsu.hpp
- graph/forest.hpp
- graph/tree_lca.hpp
- math/matrix.hpp
- misc/lis.hpp
- mod/mod.hpp (依存関係がよくわかってない。何かに使ってる?)

## 使用頻度が低いけどライブラリとしてあった方がいいヘッダー
- container/persistent_stack.hpp
- container/trie.hpp
- geo/point.hpp
- geo/graham_scan.hpp
- graph/range_edge_graph.hpp
- hash/to_hash.hpp
- misc/counting_sort.hpp
- query/mo.hpp
- query/swag.hpp
- io.hpp

## クオリティが低いので1から作り直すべきヘッダー
- container/mdarray.hpp (std::mdspanがなんか使えなかったので急造で作った)
- container/segtree.hpp (ACLのアダプタになっている)
- container/associative_treap.hpp
- container/sq_div.hpp
- graph/xor_linked_tree.hpp (いらない?)
- hash/zobrist.hpp
- math/binom.hpp
- math/xor_basis.hpp (noshi基底にしたい)
- query/functional_query.hpp
- query/polynomial_difference_array.hpp
