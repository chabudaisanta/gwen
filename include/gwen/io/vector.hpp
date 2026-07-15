#pragma once
#include <iostream>
#include <vector>

// Overloading operators for std::vector in the global namespace for convenience in competitive programming.
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        os << *it << (it == std::prev(vec.end()) ? "" : " ");
    }
    return os;
}

template <typename T>
std::istream& operator>>(std::istream& is, std::vector<T>& vec) {
    for (T& e : vec) is >> e;
    return is;
}
