#pragma once

#include <cassert>
#include <chrono>
#include <format>
#include <string>

namespace gwen {

/**
 * @brief シンプルなタイマーユーティリティ
 */
struct Timer {
public:
    using clock = std::chrono::steady_clock;
    using duration = clock::duration;
    using timepoint = clock::time_point;
    using s = std::chrono::seconds;
    using ms = std::chrono::milliseconds;
    using us = std::chrono::microseconds;

private:
    const timepoint start = clock::now();
    timepoint latest = start;
    duration limit = duration::max();

public:
    Timer() = default;

    template <typename Unit = ms> Unit elapsed() const { return std::chrono::duration_cast<Unit>(elapsed_raw()); }
    template <typename Unit = ms> Unit delta() const { return std::chrono::duration_cast<Unit>(delta_raw()); }
    template <typename Unit = ms> Unit lap() {
        auto res = delta<Unit>();
        reset();
        return res;
    }

    void reset() { latest = clock::now(); }
    void set_limit(duration target) { limit = target; }
    void restart(duration target) {
        reset();
        set_limit(target);
    }
    bool expired() const {
        assert(limit < duration::max());
        return limit <= delta_raw();
    }
    bool expired_elapsed() const {
        assert(limit < duration::max());
        return limit <= elapsed_raw();
    }

    std::string dump() const {
        return std::format("elapsed: {}ms\ndelta: {}ms\nlimit: {}ms\n", elapsed<ms>().count(), delta<ms>().count(),
                           std::chrono::duration_cast<ms>(limit).count());
    }

private:
    duration elapsed_raw() const { return clock::now() - start; }
    duration delta_raw() const { return clock::now() - latest; }
};
inline Timer timer;

}  // namespace gwen