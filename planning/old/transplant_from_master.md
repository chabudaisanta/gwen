# 移植作業をしなければいけないヘッダー一覧

**このファイルにあるヘッダーのディレクトリ構成はmasterブランチのものです。必要に応じて変更すべきです。**
使用頻度が高いヘッダー -> 使用頻度がまあまあなヘッダー -> 使用頻度が低いけどライブラリとしてあった方がいいヘッダー -> クオリティが低いので1から作り直すべきヘッダー の順で作業したい。
本当に要らないヘッダーはここにリストアップしていないため、安心してほしい。

## 使用頻度が高いヘッダー
- [x] algebra/range_affine_range_sum_monoid.hpp
- [x] algebra/rolling_hash_monoid.hpp (hash/rolling_hash.hpp内で実装済)
- [x] container/wrapper/range_add_update_sum.hpp -> query/range_add_update_sum.hpp
- [x] container/implicit_treap.hpp
- [x] container/treap.hpp
- [x] container/sorted_treap.hpp
- [x] graph/csr.hpp
- [x] graph/dijkstra.hpp
- [x] graph/hld.hpp
- [x] graph/graph.hpp
- [x] hash/rolling_hash.hpp
- [x] math/combination.hpp
- [x] math/prime.hpp
- [x] misc/compress.hpp
- [x] misc/node_pool.hpp (treap系で使用)
- [x] misc/xorshift.hpp
- [x] mod/mod61.hpp (ロリハ用)
- [x] mod/modint.hpp (dynamic_modintがprimeの中のmillerに使われてる)

## 使用頻度がまあまあなヘッダー
- [x] container/implicit_treap.hpp (重複)
- [x] container/bit_vector.hpp
- [x] graph/weighted_dsu.hpp
- [x] graph/forest.hpp
- [x] graph/tree_lca.hpp
- [x] math/matrix.hpp
- [ ] misc/lis.hpp
- [x] mod/mod.hpp (依存関係がよくわかってない。何かに使ってる?)

## 使用頻度が低いけどライブラリとしてあった方がいいヘッダー
- [ ] container/persistent_stack.hpp
- [ ] container/trie.hpp
- [ ] geo/point.hpp
- [ ] geo/graham_scan.hpp
- [ ] graph/range_edge_graph.hpp
- [ ] hash/to_hash.hpp
- [x] misc/counting_sort.hpp
- [x] query/mo.hpp
- [x] query/swag.hpp
- [x] io.hpp

## クオリティが低いので1から作り直すべきヘッダー
- [ ] container/mdarray.hpp (std::mdspanがなんか使えなかったので急造で作った)
- [x] container/segtree.hpp (ACLのアダプタになっている -> 再構築済)
- [ ] container/associative_treap.hpp
- [ ] container/sq_div.hpp
- [ ] graph/xor_linked_tree.hpp (いらない?)
- [ ] hash/zobrist.hpp
- [ ] math/binom.hpp
- [x] math/xor_basis.hpp (noshi基底にしたい)
- [ ] query/functional_query.hpp
- [ ] query/polynomial_difference_array.hpp
