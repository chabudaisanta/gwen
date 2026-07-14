#pragma once

#include <iostream>
#include <concepts>
#include <format>
#include <string>
#include <string_view>

#include "gwen/types.hpp"

namespace gwen {

namespace internal {

template<typename... Args>
constexpr bool is_empty_args(Args&&... args) {
    return sizeof...(args) == 0;
} 

template<typename T>
concept dumpable = requires(const T& t) {
    { t.dump() } -> std::convertible_to<std::string>;
};

template<typename T>
concept value_formattable = requires(const T& t) {
    { t.val() } -> std::formattable<char>;
};

usize length_of_string_view(std::string_view sv) {
    return sv.size();
}

} // namespace internal

/**
 * @brief 変数の内容を標準エラー出力 (std::cerr) に出力するユーティリティ
 * 
 * dumpable な型や value_formattable な型、および std::format で出力可能な型に対応しています。
 * 主にローカル環境でのデバッグ出力に利用されます。
 * 
 * @tparam Args 出力する変数の型
 * @param args 出力する変数のリスト
 */
template<typename... Args>
void dump(Args&&... args) {
    auto f = [](auto&& arg) {
        using T = std::remove_cvref_t<decltype(arg)>;
        if constexpr (internal::dumpable<T>) {
            return arg.dump();
        }
        else if constexpr (internal::value_formattable<T>) {
            return std::format("{}", arg.val());
        }
        else if constexpr (std::formattable<T,char>) {
            return std::format("{}", arg);
        }
        else {
            return "[unformattable token]";
        }
    };
    usize cnt = 0;
    auto sz = sizeof...(args);
    ((std::cerr << f(args) << (++cnt < sz ? ", " : "\n")), ...);
}

/**
 * @brief ローカル環境 (LOCAL マクロ定義時) でのみ動作するデバッグ出力マクロ
 * 
 * 変数名と合わせて dump 関数を呼び出します。提出時は何も出力しません。
 * 
 * @param ... 出力対象の変数（カンマ区切り）
 */
#ifdef LOCAL
#define DUMP(...) \
    do { \
        if constexpr (::gwen::internal::is_empty_args(__VA_ARGS__)) {\
            std::cerr << "empty dump called\n";\
        } else {\
            std::cerr << #__VA_ARGS__ << ": ";\
            if (15 <= ::gwen::internal::length_of_string_view(#__VA_ARGS__)) std::cerr << "\n    ";\
            ::gwen::dump(__VA_ARGS__);\
        }\
    } while(0)
#else
#define DUMP(...) do{}while(0)
#endif
} // namespace gwen