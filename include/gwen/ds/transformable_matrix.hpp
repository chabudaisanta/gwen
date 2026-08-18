#pragma once
#include <array>
#include <cassert>
#include <utility>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 内部の見た目の軸情報を管理する構造体
 */
struct AxisView {
    i32 base_axis;
    bool reversed;
};

/**
 * @brief O(1) で行・列の交換、転置、回転を行える 2 次元配列のビューアー
 * @tparam T 要素の型
 */
template <typename T> class TransformableMatrix {
private:
    std::vector<T> data_;
    std::array<i32, 2> size_;
    std::array<i32, 2> original_size_;
    std::array<std::vector<i32>, 2> order_;
    std::array<AxisView, 2> view_;

public:
    /**
     * @brief コンストラクタ
     * @param a 2次元配列
     */
    TransformableMatrix(const std::vector<std::vector<T>>& a) {
        i32 h = a.size();
        i32 w = h == 0 ? 0 : a[0].size();
        for (i32 i = 0; i < h; ++i) {
            assert(static_cast<i32>(a[i].size()) == w);
        }

        original_size_ = {h, w};
        size_ = {h, w};
        data_.reserve(h * w);
        for (i32 i = 0; i < h; ++i) {
            for (i32 j = 0; j < w; ++j) {
                data_.push_back(a[i][j]);
            }
        }

        for (i32 axis = 0; axis < 2; ++axis) {
            order_[axis].resize(size_[axis]);
            for (i32 i = 0; i < size_[axis]; ++i) {
                order_[axis][i] = i;
            }
        }
        view_ = {AxisView{0, false}, AxisView{1, false}};
    }

    /**
     * @brief 現在の見た目における指定された軸の長さを取得する
     * @param axis 軸 (0: 行, 1: 列)
     * @return 軸の長さ
     */
    inline i32 axis_len(i32 axis) const { return original_size_[view_[axis].base_axis]; }

    /**
     * @brief 現在の見た目における行数を取得する
     * @return 行数
     */
    inline i32 h() const { return axis_len(0); }

    /**
     * @brief 現在の見た目における列数を取得する
     * @return 列数
     */
    inline i32 w() const { return axis_len(1); }

    /**
     * @brief 現在の見た目におけるインデックスから、元の配列でのインデックスを取得する
     * @param axis 軸 (0: 行, 1: 列)
     * @param index 現在のインデックス
     * @return 元のインデックス
     */
    inline i32 base_index(i32 axis, i32 index) const {
        const auto& v = view_[axis];
        i32 position = v.reversed ? original_size_[v.base_axis] - 1 - index : index;
        return order_[v.base_axis][position];
    }

    /**
     * @brief 現在の見た目におけるインデックスから、現在の軸における要素の位置（並び替え前の位置）を取得する
     * @param axis 軸
     * @param index 現在のインデックス
     * @return 軸内の位置
     */
    inline i32 base_position(i32 axis, i32 index) const {
        const auto& v = view_[axis];
        return v.reversed ? original_size_[v.base_axis] - 1 - index : index;
    }

    /**
     * @brief 指定した要素の参照を取得する
     * @param i 行番号
     * @param j 列番号
     * @return 要素への参照
     */
    inline T& operator()(i32 i, i32 j) {
        assert(0 <= i && i < h() && 0 <= j && j < w());
        std::array<i32, 2> p;
        p[view_[0].base_axis] = base_index(0, i);
        p[view_[1].base_axis] = base_index(1, j);
        return data_[p[0] * original_size_[1] + p[1]];
    }

    /**
     * @brief 指定した要素の const 参照を取得する
     * @param i 行番号
     * @param j 列番号
     * @return 要素への const 参照
     */
    inline const T& operator()(i32 i, i32 j) const {
        assert(0 <= i && i < h() && 0 <= j && j < w());
        std::array<i32, 2> p;
        p[view_[0].base_axis] = base_index(0, i);
        p[view_[1].base_axis] = base_index(1, j);
        return data_[p[0] * original_size_[1] + p[1]];
    }

    /**
     * @brief 現在の i 行目と j 行目を交換する
     * @param i 行番号
     * @param j 行番号
     */
    inline void swap_rows(i32 i, i32 j) {
        assert(0 <= i && i < h() && 0 <= j && j < h());
        i32 axis = view_[0].base_axis;
        std::swap(order_[axis][base_position(0, i)], order_[axis][base_position(0, j)]);
    }

    /**
     * @brief 現在の i 列目と j 列目を交換する
     * @param i 列番号
     * @param j 列番号
     */
    inline void swap_columns(i32 i, i32 j) {
        assert(0 <= i && i < w() && 0 <= j && j < w());
        i32 axis = view_[1].base_axis;
        std::swap(order_[axis][base_position(1, i)], order_[axis][base_position(1, j)]);
    }

    /**
     * @brief 転置する
     */
    inline void transpose() { std::swap(view_[0], view_[1]); }

    /**
     * @brief 時計回りに 90 度ずつ回転する
     * @param num 回転回数
     */
    inline void rotate(i32 num = 1) {
        num %= 4;
        if (num < 0) num += 4;
        for (i32 k = 0; k < num; ++k) {
            auto old_row = view_[0];
            view_[0] = view_[1];
            view_[1] = old_row;
            view_[1].reversed = !view_[1].reversed;
        }
    }

    /**
     * @brief 現在の見た目における 2次元配列を取得する
     * @return 2次元配列
     */
    std::vector<std::vector<T>> to_vec() const {
        i32 r = h();
        i32 c = w();
        std::vector<std::vector<T>> res(r, std::vector<T>(c));
        for (i32 i = 0; i < r; ++i) {
            for (i32 j = 0; j < c; ++j) {
                res[i][j] = (*this)(i, j);
            }
        }
        return res;
    }
};

}  // namespace gwen
