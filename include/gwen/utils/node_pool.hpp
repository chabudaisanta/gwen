#pragma once

#include <cassert>
#include <utility>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 動的確保の代わりに配列でノードを管理するメモリプール
 * @details 削除されたノードのIDを再利用する機能を持ちます。
 * 
 * @tparam Node ノードの型
 */
template <typename Node>
class NodePool {
private:
    std::vector<Node> pool;
    std::vector<i32> free_list;

public:
    /**
     * @brief デフォルトコンストラクタ
     * @details プールの最初の要素（ID=0）にダミーノードを配置します（1-indexed用やNULL扱いとして利用可能）。
     */
    explicit NodePool() {
        pool.emplace_back();
    }

    /**
     * @brief 新しいノードを確保し、そのIDを返します（右辺値参照）。
     * @details 計算量: 償却 \f$O(1)\f$
     * @param node 確保するノード（ムーブされます）
     * @return 確保されたノードのID
     */
    i32 new_node(Node&& node) {
        if (!free_list.empty()) {
            i32 id = free_list.back();
            free_list.pop_back();
            pool[id] = std::move(node);
            return id;
        }
        pool.emplace_back(std::move(node));
        return static_cast<i32>(pool.size() - 1);
    }

    /**
     * @brief 新しいノードを確保し、そのIDを返します（左辺値参照）。
     * @details 計算量: 償却 \f$O(1)\f$
     * @param node 確保するノード（コピーされます）
     * @return 確保されたノードのID
     */
    i32 new_node(const Node& node) {
        if (!free_list.empty()) {
            i32 id = free_list.back();
            free_list.pop_back();
            pool[id] = node;
            return id;
        }
        pool.push_back(node);
        return static_cast<i32>(pool.size() - 1);
    }

    /**
     * @brief 指定したIDのノードを解放し、再利用可能にします。
     * @details 解放されたノードはデフォルト構築状態にリセットされます。
     * 計算量: \f$O(1)\f$
     * @param id 解放するノードのID
     * @pre `id` は有効なノードIDであり、すでに解放済みでないこと。
     */
    void free_node(i32 id) {
        assert(id > 0 && id < static_cast<i32>(pool.size()));
        free_list.push_back(id);
        pool[id] = Node(); // 参照やリソースのリークを防ぐためリセット
    }

    /**
     * @brief プール内の有効な総ノード数を返します（ダミーノードを含む）。
     * @details 計算量: \f$O(1)\f$
     * @return 有効なノード数
     */
    i32 size() const {
        return static_cast<i32>(pool.size()) - static_cast<i32>(free_list.size());
    }

    /**
     * @brief 指定したIDのノードへアクセスします。
     * @param id ノードのID
     * @return ノードへの参照
     */
    Node& operator[](i32 id) {
        assert(id >= 0 && id < static_cast<i32>(pool.size()));
        return pool[id];
    }

    /**
     * @brief 指定したIDのノードへアクセスします。
     * @param id ノードのID
     * @return ノードへの const 参照
     */
    const Node& operator[](i32 id) const {
        assert(id >= 0 && id < static_cast<i32>(pool.size()));
        return pool[id];
    }
};

} // namespace gwen
