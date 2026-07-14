#pragma once

#include <iostream>
#include <random>
#include <optional>
#include "testlib.h"

namespace gwen::test {

/**
 * @brief testlib.h の rnd にシードを設定し、使用されたシード値を標準エラー出力に記録します。
 * テスト失敗時にログからシード値を拾い、固定シードで再実行することでバグを再現可能にします。
 * 
 * @param fixed_seed デバッグ用にシードを固定したい場合は数値を渡します。
 *                   指定がない場合は std::random_device によりランダムなシードが生成されます。
 * @return 実際に設定されたシード値
 */
inline uint32_t setup_random_seed(std::optional<uint32_t> fixed_seed = std::nullopt) {
    uint32_t seed;
    if (fixed_seed.has_value()) {
        seed = fixed_seed.value();
    } else {
        seed = std::random_device{}();
    }
    // テスト失敗時にもコンソールログに確実に残るよう std::cerr に出力
    std::cerr << "[   INFO   ] testlib rnd initialized with seed: " << seed << std::endl;
    rnd.setSeed(seed);
    return seed;
}

} // namespace gwen::test
