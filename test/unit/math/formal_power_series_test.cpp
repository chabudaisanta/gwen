#include "gwen/math/formal_power_series.hpp"

#include <gtest/gtest.h>

#include <atomic>
#include <random>
#include <thread>
#include <vector>

using namespace gwen;

namespace {

using mint = modint998244353;
using fps = FormalPowerSeries<mint>;

std::vector<mint> coefficients(const fps& f) {
    std::vector<mint> res(static_cast<usize>(f.size()));
    for (i32 i = 0; i < f.size(); ++i) res[static_cast<usize>(i)] = f[i];
    return res;
}

std::vector<mint> naive_convolution(const std::vector<mint>& a, const std::vector<mint>& b) {
    if (a.empty() || b.empty()) return {};
    std::vector<mint> res(a.size() + b.size() - 1);
    for (usize i = 0; i < a.size(); ++i) {
        for (usize j = 0; j < b.size(); ++j) res[i + j] += a[i] * b[j];
    }
    return res;
}

}  // namespace

static_assert(field<mint>);
static_assert(field<f64>);

TEST(FormalPowerSeriesTest, ConstructionAndPrefix) {
    fps f{1, 2, 0, 4};
    EXPECT_EQ(f.size(), 4);
    EXPECT_FALSE(f.empty());
    EXPECT_EQ(f[1], mint(2));

    f[1] = mint(3);
    EXPECT_EQ(coefficients(f.prefix(2)), (std::vector<mint>{1, 3}));
    EXPECT_EQ(coefficients(f.prefix(10)), (std::vector<mint>{1, 3, 0, 4}));
    EXPECT_TRUE(f.prefix(0).empty());
}

TEST(FormalPowerSeriesTest, BasicOperations) {
    const fps f{1, 2, 3};
    const fps g{4, 5};
    EXPECT_EQ(coefficients(f + g), (std::vector<mint>{5, 7, 3}));
    EXPECT_EQ(coefficients(f - g), (std::vector<mint>{mint(-3), mint(-3), 3}));
    EXPECT_EQ(coefficients(-f), (std::vector<mint>{mint(-1), mint(-2), mint(-3)}));
    EXPECT_EQ(coefficients(f * g), (std::vector<mint>{4, 13, 22, 15}));
    EXPECT_EQ(coefficients(f * mint(2)), (std::vector<mint>{2, 4, 6}));
    EXPECT_EQ(coefficients(mint(2) * f), (std::vector<mint>{2, 4, 6}));
    EXPECT_EQ(coefficients((f * mint(6)) / mint(3)), (std::vector<mint>{2, 4, 6}));
    EXPECT_TRUE((fps{} * f).empty());
    EXPECT_TRUE((f * fps{}).empty());
}

TEST(FormalPowerSeriesTest, DerivativeAndIntegral) {
    const fps f{3, 4, 5, 6};
    EXPECT_EQ(coefficients(f.derivative()), (std::vector<mint>{4, 10, 18}));
    EXPECT_EQ(coefficients(f.derivative().integral()), (std::vector<mint>{0, 4, 5, 6}));

    const FormalPowerSeries<f64> x{0.0, 1.0, 0.5};
    const auto integral = x.derivative().integral();
    EXPECT_DOUBLE_EQ(integral[0], 0.0);
    EXPECT_DOUBLE_EQ(integral[1], 1.0);
    EXPECT_DOUBLE_EQ(integral[2], 0.5);
}

TEST(FormalPowerSeriesTest, ZeroLengthAdvancedOperations) {
    EXPECT_TRUE(fps{}.inv(0).empty());
    EXPECT_TRUE(fps{}.log(0).empty());
    EXPECT_TRUE(fps{}.exp(0).empty());
}

TEST(FormalPowerSeriesTest, SparseInverseLogAndExp) {
    const fps f{1, 2, 0, 3};
    const auto inverse = f.inv(20);
    const auto product = f * inverse;
    ASSERT_EQ(inverse.size(), 20);
    EXPECT_EQ(product[0], mint(1));
    for (i32 i = 1; i < 20; ++i) EXPECT_EQ(product[i], mint(0));

    const fps h{0, 1, 0, 2};
    const auto round_trip = h.exp(20).log(20);
    ASSERT_EQ(round_trip.size(), 20);
    for (i32 i = 0; i < 20; ++i) EXPECT_EQ(round_trip[i], i < h.size() ? h[i] : mint(0));

    const FormalPowerSeries<f64> x{0.0, 1.0};
    const auto exponential = x.exp(6);
    EXPECT_NEAR(exponential[0], 1.0, 1e-12);
    EXPECT_NEAR(exponential[1], 1.0, 1e-12);
    EXPECT_NEAR(exponential[2], 0.5, 1e-12);
    EXPECT_NEAR(exponential[3], 1.0 / 6.0, 1e-12);
    EXPECT_NEAR(exponential[4], 1.0 / 24.0, 1e-12);
    EXPECT_NEAR(exponential[5], 1.0 / 120.0, 1e-12);
}

TEST(FormalPowerSeriesTest, NttConvolutionMatchesNaive) {
    std::mt19937 rng(1234567);
    for (const auto [n, m] : {std::pair{59, 61}, std::pair{60, 60}, std::pair{61, 61}, std::pair{97, 131}}) {
        std::vector<mint> a(static_cast<usize>(n)), b(static_cast<usize>(m));
        for (auto& x : a) x = rng();
        for (auto& x : b) x = rng();
        EXPECT_EQ(coefficients(fps(a) * fps(b)), naive_convolution(a, b));
    }
}

TEST(FormalPowerSeriesTest, NttSquareMatchesNaive) {
    std::mt19937 rng(2345678);
    std::vector<mint> a(193);
    for (auto& x : a) x = rng();
    fps f(a);
    f *= f;
    EXPECT_EQ(coefficients(f), naive_convolution(a, a));
}

TEST(FormalPowerSeriesTest, DenseInverseLogAndExp) {
    std::mt19937 rng(3456789);
    for (i32 n : {62, 63, 64, 65, 127, 128, 129, 257}) {
        std::vector<mint> a(static_cast<usize>(n));
        a[0] = mint(0);
        for (i32 i = 1; i < n; ++i) a[static_cast<usize>(i)] = mint(rng() % 100 + 1);
        const fps f(a);
        const auto exponential = f.exp(n);
        const auto logarithm = exponential.log(n);
        ASSERT_EQ(exponential.size(), n);
        ASSERT_EQ(logarithm.size(), n);
        for (i32 i = 0; i < n; ++i) EXPECT_EQ(logarithm[i], f[i]);

        a[0] = mint(1);
        const fps unit(a);
        const auto inverse = unit.inv(n);
        const auto product = unit * inverse;
        EXPECT_EQ(product[0], mint(1));
        for (i32 i = 1; i < n; ++i) EXPECT_EQ(product[i], mint(0));

        const auto recovered = unit.log(n).exp(n);
        for (i32 i = 0; i < n; ++i) EXPECT_EQ(recovered[i], unit[i]);
    }
}

TEST(FormalPowerSeriesTest, ConcurrentCallsDoNotShareWorkspace) {
    std::atomic<bool> valid = true;
    auto check = [&valid](u32 seed) {
        std::mt19937 rng(seed);
        std::vector<mint> a(257);
        for (usize i = 1; i < a.size(); ++i) a[i] = mint(rng() % 100 + 1);
        const fps f(a);
        const auto actual = f.exp(257).log(257);
        for (i32 i = 0; i < actual.size(); ++i) {
            if (actual[i] != f[i]) valid.store(false);
        }
    };
    std::jthread first(check, 4567890);
    std::jthread second(check, 5678901);
    first.join();
    second.join();
    EXPECT_TRUE(valid.load());
}

#ifndef NDEBUG
TEST(FormalPowerSeriesDeathTest, RejectsInvalidPreconditions) {
    EXPECT_DEATH((void)fps{}.prefix(-1), "");
    EXPECT_DEATH((void)fps{}.inv(1), "");
    EXPECT_DEATH(((void)fps{0, 1}.inv(2)), "");
    EXPECT_DEATH(((void)fps{2, 1}.log(2)), "");
    EXPECT_DEATH(((void)fps{1, 1}.exp(2)), "");
    EXPECT_DEATH((void)(fps{1} / mint(0)), "");
}
#endif
