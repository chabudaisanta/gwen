#pragma once

#include <concepts>
#include <format>
#include <iostream>
#include <source_location>
#include <string>
#include <string_view>
#if __has_include(<unistd.h>)
#include <unistd.h>
#endif

#include "gwen/types.hpp"

namespace gwen {

namespace internal {

constexpr std::string_view basename(std::string_view path) {
    auto pos = path.find_last_of("/\\");
    return pos == std::string_view::npos ? path : path.substr(pos + 1);
}

template <typename... Args> constexpr bool is_empty_args(Args&&... args) { return sizeof...(args) == 0; }

template <typename T>
concept dumpable = requires(const T& t) {
    { t.dump() } -> std::convertible_to<std::string>;
};

template <typename T>
concept value_formattable = requires(const T& t) {
    { t.val() } -> std::formattable<char>;
};

struct Color {
    const char* code;
    friend std::ostream& operator<<(std::ostream& os, const Color& c) {
#if __has_include(<unistd.h>)
        static const bool tty = ::isatty(2);
        if (tty) os << c.code;
#else
        os << c.code;
#endif
        return os;
    }
};

constexpr Color RED{"\033[1;31m"};
constexpr Color GREEN{"\033[1;32m"};
constexpr Color YELLOW{"\033[1;33m"};
constexpr Color BLUE{"\033[1;34m"};
constexpr Color MAGENTA{"\033[1;35m"};
constexpr Color CYAN{"\033[1;36m"};
constexpr Color WHITE{"\033[1;37m"};
constexpr Color RESET{"\033[0m"};

}  // namespace internal

/**
 * @brief 変数の内容を標準エラー出力 (std::cerr) に出力するユーティリティ
 *
 * dumpable な型や value_formattable な型、および std::format で出力可能な型に対応しています。
 * 主にローカル環境でのデバッグ出力に利用されます。
 *
 * @tparam Args 出力する変数の型
 * @param args 出力する変数のリスト
 */
template <typename... Args> void dump(Args&&... args) {
    auto f = [](auto&& arg) {
        using T = std::remove_cvref_t<decltype(arg)>;
        if constexpr (internal::dumpable<T>) {
            return arg.dump();
        }
        else if constexpr (internal::value_formattable<T>) {
            return std::format("{}", arg.val());
        }
        else if constexpr (std::formattable<T, char>) {
            return std::format("{}", arg);
        }
        else {
            return "[unformattable token]";
        }
    };
    usize cnt = 0;
    auto sz = sizeof...(args);
    ((std::cerr << f(args) << (++cnt < sz ? ",\n" : "\n")), ...);
}

/**
 * @brief ローカル環境 (LOCAL マクロ定義時) でのみ動作するデバッグ出力マクロ
 *
 * 変数名と合わせて dump 関数を呼び出します。提出時は何も出力しません。
 *
 * @param ... 出力対象の変数（カンマ区切り）
 */
#ifdef LOCAL
#define DUMP(...)                                                                                                     \
    do {                                                                                                              \
        auto loc = std::source_location::current();                                                                   \
        std::cerr << ::gwen::internal::CYAN << "[gwen::DUMP @ " << ::gwen::internal::basename(loc.file_name()) << ':' \
                  << loc.line() << "]\n"                                                                              \
                  << ::gwen::internal::RESET;                                                                         \
        constexpr std::string_view __vars_sv = #__VA_ARGS__;                                                          \
        if constexpr (__vars_sv.empty() || __vars_sv.find_first_not_of(" \t\r\n") == std::string_view::npos) {        \
            std::cerr << ::gwen::internal::RED << "[empty]\n" << ::gwen::internal::RESET;                             \
        }                                                                                                             \
        else {                                                                                                        \
            std::cerr << ::gwen::internal::YELLOW << "[vars]\n"                                                       \
                      << ::gwen::internal::RESET << #__VA_ARGS__ << "\n"                                              \
                      << ::gwen::internal::GREEN << "[dump]\n"                                                        \
                      << ::gwen::internal::RESET;                                                                     \
            ::gwen::dump(__VA_ARGS__);                                                                                \
        }                                                                                                             \
    } while (0)
#else
#define DUMP(...) \
    do {          \
    } while (0)
#endif
}  // namespace gwen