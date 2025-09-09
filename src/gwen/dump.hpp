#pragma once

#include <iostream>
#include <string>
#include <concepts>
#include <utility>
#include <tuple>
#include <vector>
#include <array>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#ifdef LOCAL
#define DUMP(arg)       std::cerr << #arg << ": " << gwen::to_str(arg) << '\n'
#define DEBUG(arg)      std::cerr << #arg << ": " << gwen::to_str(arg) << '\n'
#else
#define DUMP(arg)       void(0)
#define DEBUG(arg)      void(0)
#endif

namespace gwen {

std::string to_str(bool b) { return b ? "true" : "false"; }

std::string to_str(const std::string& str) { return str; }
std::string to_str(char c) { return std::string{c}; }
std::string to_str(const char* s) { return std::string{s}; }

template<std::integral T>
std::string to_str(T x) { return std::to_string(x); }

template<std::floating_point T>
std::string to_str(T x) { return std::to_string(x); }

template<typename T>
requires requires(const T& t) { t.val(); }
std::string to_str(const T& x) { return to_str(x.val()); }

template<typename T>
requires requires(const T& t) { t.dump(); }
std::string to_str(const T& x) { return to_str(x.dump()); }

template<typename T, typename S>
std::string to_str(const std::pair<T,S>& p) { return std::string{'('} + to_str(p.first) + ", " + to_str(p.second) + ')'; }

template<std::size_t I = 0, typename... Ts>
std::string tuple_to_str_impl(const std::tuple<Ts...>& t) {
    if constexpr (I == sizeof...(Ts)) {
        return "";
    } else {
        std::string ret = gwen::to_str(std::get<I>(t));
        if constexpr (I + 1 < sizeof...(Ts)) ret += ", ";
        return ret + tuple_to_str_impl<I + 1>(t);
    }
}
template<typename... Ts>
std::string to_str(const std::tuple<Ts...>& t) {
    return "(" + tuple_to_str_impl(t) + ")";
}

template<typename Iterator>
std::string to_str(Iterator begin, Iterator end, const std::string& partition = " ", char pb = '\0', char pe = '\0') {
    std::string ret;
    if(pb) ret += pb;
    for(auto it = begin; it != end; ++it) {
        if(it != begin) ret += partition;
        ret += to_str(*it);
    }
    if(pe) ret += pe;
    return ret;
}

template<typename T>
std::string to_str(const std::vector<T>& vec) { return to_str(vec.begin(), vec.end(), ", ", '[', ']'); }
template<typename T, std::size_t N>
std::string to_str(const std::array<T,N>& arr) { return to_str(arr.begin(), arr.end(),  ", ", '[', ']'); }
template<typename T>
std::string to_str(const std::list<T>& ls) { return to_str(ls.begin(), ls.end(), ", ", '[', ']'); }
template<typename T>
std::string to_str(const std::deque<T>& dq) { return to_str(dq.begin(), dq.end(), ", ", '[', ']'); }

template<typename T>
std::string to_str(const std::set<T>& se) { return to_str(se.begin(), se.end(), ", ", '{', '}'); }
template<typename T>
std::string to_str(const std::multiset<T>& ms) { return to_str(ms.begin(), ms.end(), ", ", '{', '}'); }
template<typename T>
std::string to_str(const std::unordered_set<T>& us) { return to_str(us.begin(), us.end(), ", ", '{', '}'); }
template<typename T>
std::string to_str(const std::unordered_multiset<T>& um) { return to_str(um.begin(), um.end(), ", ", '{', '}'); }

template<typename K, typename V>
std::string to_str(const std::map<K,V>& mp) {
    std::string ret;
    ret += '{';
    for(auto it = mp.begin(); it != mp.end(); ++it) {
        if(it != mp.begin()) ret += ", ";
        ret += std::string{'('} + to_str(it->first) + ": " + to_str(it->second) + ')';
    }
    ret += '}';
    return ret;
}
template<typename K, typename V>
std::string to_str(const std::unordered_map<K,V>& mp) {
    std::string ret;
    ret += '{';
    for(auto it = mp.begin(); it != mp.end(); ++it) {
        if(it != mp.begin()) ret += ", ";
        ret += std::string{'('} + to_str(it->first) + ": " + to_str(it->second) + ')';
    }
    ret += '}';
    return ret;
}

} // namespace gwen