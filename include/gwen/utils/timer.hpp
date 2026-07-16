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
    /** @brief タイマーを初期化し、計測を開始します */
    Timer() = default;

    /** @brief 生成時からの経過時間を指定した単位で取得します */
    template <typename Unit = ms> Unit elapsed() const { return std::chrono::duration_cast<Unit>(elapsed_raw()); }

    /** @brief 最後に reset または lap を呼んでからの経過時間を取得します */
    template <typename Unit = ms> Unit delta() const { return std::chrono::duration_cast<Unit>(delta_raw()); }

    /** @brief delta() の時間を返し、同時にタイマー（ラップ用の時刻）をリセットします */
    template <typename Unit = ms> Unit lap() {
        auto res = delta<Unit>();
        reset();
        return res;
    }

    /** @brief ラップ用の時刻 (latest) を現在時刻に更新します */
    void reset() { latest = clock::now(); }

    /** @brief 制限時間 (limit) を設定します */
    void set_limit(duration target) { limit = target; }

    /** @brief ラップ時刻をリセットし、同時に制限時間を設定します */
    void restart(duration target) {
        reset();
        set_limit(target);
    }

    /** @brief reset または lap から制限時間を超過したか判定します */
    bool expired() const {
        assert(limit < duration::max());
        return limit <= delta_raw();
    }

    /** @brief 生成時から制限時間を超過したか判定します */
    bool expired_elapsed() const {
        assert(limit < duration::max());
        return limit <= elapsed_raw();
    }

    /** @brief タイマーの現在状態を文字列として出力します (ダンプ用) */
    std::string dump() const {
        return std::format("Timer{{\n  elapsed: {}ms\n  delta: {}ms\n  limit: {}ms\n}}", elapsed<ms>().count(),
                           delta<ms>().count(), std::chrono::duration_cast<ms>(limit).count());
    }

private:
    duration elapsed_raw() const { return clock::now() - start; }
    duration delta_raw() const { return clock::now() - latest; }
};
inline Timer timer;

}  // namespace gwen