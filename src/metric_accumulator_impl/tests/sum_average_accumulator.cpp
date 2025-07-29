#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <gtest/gtest.h>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(TestSumAverageAccumulator, TestAccumulate) {
    // given
    SumAverageAccumulator acc;
    const metric::MetricResult given_metric_result{"some metric", 42};
    const SumAverageAccumulator::SumAverage expected_value{static_cast<int>(given_metric_result.metric_value),
                                                           static_cast<double>(given_metric_result.metric_value)};

    // when
    acc.Accumulate(given_metric_result);
    acc.Finalize();
    const auto got_value = acc.Get();

    // then
    EXPECT_EQ(got_value.sum, expected_value.sum);
    EXPECT_DOUBLE_EQ(got_value.average, expected_value.average);
}

TEST(TestSumAverageAccumulator, TestTwiceAccumulate) {
    // given
    SumAverageAccumulator acc;
    const metric::MetricResult given_metric_result{"another metric", 17};
    const SumAverageAccumulator::SumAverage expected_value{static_cast<int>(2 * given_metric_result.metric_value),
                                                           static_cast<double>(given_metric_result.metric_value)};

    // when
    acc.Accumulate(given_metric_result);
    acc.Accumulate(given_metric_result);
    acc.Finalize();
    const auto got_value = acc.Get();

    // then
    EXPECT_EQ(got_value, expected_value);
}

TEST(TestSumAverageAccumulator, TestDoubleAccumulate) {
    // given
    SumAverageAccumulator acc;
    constexpr int num_values = 10;
    constexpr double eps = 1e-6;
    constexpr SumAverageAccumulator::SumAverage expected_value{45, 4.5};

    // when
    for (int i = 0; i < num_values; ++i) {
        acc.Accumulate({"best metric", static_cast<metric::MetricResult::ValueType>(i)});
    }
    acc.Finalize();
    const auto got_value = acc.Get();

    // then
    EXPECT_EQ(got_value.sum, expected_value.sum);
    EXPECT_TRUE(std::abs(got_value.average - expected_value.average) < eps);
}

TEST(TestSumAverageAccumulator, TestAccumulateAfterReset) {
    // given
    SumAverageAccumulator acc;
    const metric::MetricResult given_metric_result{"given metric", 324};
    const SumAverageAccumulator::SumAverage expected_value{static_cast<int>(given_metric_result.metric_value),
                                                           static_cast<double>(given_metric_result.metric_value)};

    // when
    for (int i = 0; i < 10; ++i) {
        acc.Accumulate({"yet another metric", static_cast<metric::MetricResult::ValueType>(i)});
    }
    acc.Finalize();
    acc.Reset();

    acc.Accumulate(given_metric_result);

    acc.Finalize();
    const auto got_value = acc.Get();

    // then
    EXPECT_EQ(got_value, expected_value);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
