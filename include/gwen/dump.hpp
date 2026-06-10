#pragma once

#ifdef LOCAL
#include <gwen/dump_to_str.hpp>
#define DUMP(...) std::cerr << #__VA_ARGS__ << ": " << gwen::to_str_variadic(__VA_ARGS__) << '\n'
#else
#define DUMP(...) void(0)
#endif
