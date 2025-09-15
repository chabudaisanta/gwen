#pragma once

#include <algorithm>
#include <array>
#include <concepts>
#include <deque>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {

//-------------------------
// standard types
//-------------------------
constexpr inline std::string to_str() { return ""; }
constexpr inline std::string to_str(bool b) { return b ? "true" : "false"; }
constexpr inline std::string to_str(char c) { return std::string{c}; }
constexpr inline std::string to_str(const char* cs) { return std::string{cs}; }
constexpr inline std::string to_str(const std::string& str) { return str; }
constexpr inline std::string to_str(std::nullptr_t) { return "nullptr"; }
constexpr inline std::string to_str(u128 x) {
    if (x == 0) return "0";
    std::string ret;
    while (x) {
        ret.push_back(static_cast<char>('0' + (x % 10)));
        x /= 10;
    }
    std::reverse(ret.begin(), ret.end());
    return ret;
}
constexpr inline std::string to_str(i128 x) {
    if (x < 0) return std::string{'-'} + to_str(static_cast<u128>(-x));
    return to_str(static_cast<u128>(x));
}

template <std::integral T> std::string to_str(T x) { return std::to_string(x); }
template <std::floating_point T> std::string to_str(T f) {
    return std::to_string(f);
}

//-------------------------
// prototype
//-------------------------

// pair / tuple
template <typename T1, typename T2>
std::string to_str(const std::pair<T1, T2>& p);
template <typename... Ts> std::string to_str(const std::tuple<Ts...>& t);

// input iterator helper
template <std::input_iterator Iterator>
std::string to_str(Iterator begin,
                   Iterator end,
                   const std::string& partition = ", ",
                   char pb = '\0',
                   char pe = '\0');

// sequence containers
template <typename T> std::string to_str(const std::vector<T>& sc);
template <typename T, std::size_t N>
std::string to_str(const std::array<T, N>& sc);
template <typename T> std::string to_str(const std::deque<T>& sc);
template <typename T, std::size_t N> std::string to_str(const T (&sc)[N]);

/* さすがに要らなそう
template<typename T>
std::string to_str(const std::list<T>& sc);
template<typename T>
std::string to_str(const std::forward_list<T>& sc);
*/

// set containers
template <typename T> std::string to_str(const std::set<T>& se);
template <typename T> std::string to_str(const std::multiset<T>& se);
template <typename T> std::string to_str(const std::unordered_set<T>& se);
template <typename T> std::string to_str(const std::unordered_multiset<T>& se);

// map containers
template <typename K, typename V> std::string to_str(const std::map<K, V>& mp);
template <typename K, typename V>
std::string to_str(const std::multimap<K, V>& mp);
template <typename K, typename V>
std::string to_str(const std::unordered_map<K, V>& mp);
template <typename K, typename V>
std::string to_str(const std::unordered_multimap<K, V>& mp);
template <std::input_iterator Iterator>
std::string to_str_map_helper(Iterator begin, Iterator end);

// user-defined
template <typename T>
    requires requires(const T& t) { gwen::to_str(t.val()); }
std::string to_str(const T& t);
template <typename T>
    requires requires(const T& t) { gwen::to_str(t.dump()); }
std::string to_str(const T& t);

//-------------------------
// implementation
//-------------------------

// pair / tuple
template <typename T1, typename T2>
std::string to_str(const std::pair<T1, T2>& p) {
    return std::string{'('} + to_str(p.first) + ", " + to_str(p.second) + ')';
}
template <typename... Ts> std::string to_str(const std::tuple<Ts...>& t) {
    std::string ret{'('};
    bool first = true;
    std::apply(
        [&](const auto&... args) {
            ((ret += (first ? "" : ", "), ret += gwen::to_str(args),
              first = false),
             ...);
        },
        t);
    return ret + ')';
}

template <typename... Args>
inline std::string to_str_variadic(const Args&... args) {
    std::string ret;
    std::size_t index = 0;

    auto process_one = [&](const auto& arg) {
        if (index++ > 0) {
            ret += ", ";
        }

        if constexpr (requires { gwen::to_str(arg); }) {
            ret += gwen::to_str(arg);
        } else {
            ret += "[?]";
        }
    };

    (process_one(args), ...);

    return ret;
}

// input iterator helper
template <std::input_iterator Iterator>
std::string to_str(Iterator begin,
                   Iterator end,
                   const std::string& partition,
                   char pb,
                   char pe) {
    std::string ret;
    if (pb) ret += pb;
    for (auto it = begin; it != end; ++it) {
        if (it != begin) ret += partition;
        ret += to_str(*it);
    }
    if (pe) ret += pe;
    return ret;
}

// sequence containers
template <typename T> std::string to_str(const std::vector<T>& sc) {
    return to_str(sc.begin(), sc.end(), ", ", '[', ']');
}
template <typename T, std::size_t N>
std::string to_str(const std::array<T, N>& sc) {
    return to_str(sc.begin(), sc.end(), ", ", '[', ']');
}
template <typename T> std::string to_str(const std::deque<T>& sc) {
    return to_str(sc.begin(), sc.end(), ", ", '[', ']');
}
template <typename T, std::size_t N> std::string to_str(const T (&sc)[N]) {
    return to_str(sc.begin(), sc.end(), ", ", '[', ']');
}

// set containers
template <typename T> std::string to_str(const std::set<T>& se) {
    return to_str(se.begin(), se.end(), ", ", '{', '}');
}
template <typename T> std::string to_str(const std::multiset<T>& se) {
    return to_str(se.begin(), se.end(), ", ", '{', '}');
}
template <typename T> std::string to_str(const std::unordered_set<T>& se) {
    return to_str(se.begin(), se.end(), ", ", '{', '}');
}
template <typename T> std::string to_str(const std::unordered_multiset<T>& se) {
    return to_str(se.begin(), se.end(), ", ", '{', '}');
}

// map containers
template <typename K, typename V> std::string to_str(const std::map<K, V>& mp) {
    return to_str_map_helper(mp.begin(), mp.end());
}
template <typename K, typename V>
std::string to_str(const std::multimap<K, V>& mp) {
    return to_str_map_helper(mp.begin(), mp.end());
}
template <typename K, typename V>
std::string to_str(const std::unordered_map<K, V>& mp) {
    return to_str_map_helper(mp.begin(), mp.end());
}
template <typename K, typename V>
std::string to_str(const std::unordered_multimap<K, V>& mp) {
    return to_str_map_helper(mp.begin(), mp.end());
}
template <std::input_iterator Iterator>
std::string to_str_map_helper(Iterator begin, Iterator end) {
    std::string ret{'{'};
    for (auto it = begin; it != end; ++it) {
        if (it != begin) ret += ", ";
        ret += '(' + to_str(it->first) + ": " + to_str(it->second) + ')';
    }
    ret += '}';
    return ret;
}

// user-defined
template <typename T>
    requires requires(const T& t) { gwen::to_str(t.val()); }
std::string to_str(const T& t) {
    return gwen::to_str(t.val());
}
template <typename T>
    requires requires(const T& t) { gwen::to_str(t.dump()); }
std::string to_str(const T& t) {
    return gwen::to_str(t.dump());
}

}  // namespace gwen

#ifdef LOCAL
#define DEBUG(...)                                                          \
    std::cerr << #__VA_ARGS__ << ": " << gwen::to_str_variadic(__VA_ARGS__) \
              << '\n'
#define DUMP(...)                                                           \
    std::cerr << #__VA_ARGS__ << ": " << gwen::to_str_variadic(__VA_ARGS__) \
              << '\n'
#else
#define DEBUG(...) void(0)
#define DUMP(...) void(0)
#endif

// std::string += std::string は、atcoder環境なら線形時間でやってくれそう
// https://atcoder.jp/contests/abc379/submissions/69207872