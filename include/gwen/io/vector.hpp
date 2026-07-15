#pragma once
#include <iostream>
#include <vector>

// Overloading operators for std::vector in the global namespace for convenience in competitive programming.
/**
 * @brief std::vector の要素を空白区切りで出力ストリームに書き込む
 * @tparam T ベクターの要素型
 * @param os 出力ストリーム
 * @param vec 出力対象のベクター
 * @return std::ostream& 出力後のストリーム
 */
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        os << *it << (it == std::prev(vec.end()) ? "" : " ");
    }
    return os;
}

/**
 * @brief 入力ストリームから std::vector の全要素に読み込む
 * @tparam T ベクターの要素型
 * @param is 入力ストリーム
 * @param vec 入力対象のベクター
 * @return std::istream& 入力後のストリーム
 */
template <typename T>
std::istream& operator>>(std::istream& is, std::vector<T>& vec) {
    for (T& e : vec) is >> e;
    return is;
}
