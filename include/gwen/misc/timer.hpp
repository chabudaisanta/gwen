#pragma once

#include <cassert>
#include <chrono>

namespace gwen {

struct Timer {
    using clock = std::chrono::steady_clock;
};
inline Timer timer;

}  // namespace gwen