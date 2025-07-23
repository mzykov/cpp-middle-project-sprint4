#include "metric_accumulator_impl/average_accumulator.hpp"

#include <gtest/gtest.h>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(TestAverageAccumulator, TestAccumulateSimple) {
    // given
    AverageAccumulator acc;
    const metric::MetricResult given_metric_result{"some metric", 42};

    // when
    acc.Accumulate(given_metric_result);
    acc.Finalize();
    const auto got_value = acc.Get();

    // then
    EXPECT_EQ(got_value, given_metric_result.metric_value);
}

TEST(TestAverageAccumulator, TestAccumulateResultOfTypeInteger) {
    // given
    AverageAccumulator acc;
    const metric::MetricResult given_metric_result{"another metric", 17};

    // when
    acc.Accumulate(given_metric_result);
    acc.Accumulate(given_metric_result);
    acc.Finalize();
    const auto got_value = acc.Get();

    // then
    EXPECT_EQ(got_value, given_metric_result.metric_value);
}

TEST(TestAverageAccumulator, TestAccumulateResultOfTypeDouble) {
    // given
    AverageAccumulator acc;
    constexpr double expected_value = 4.5;

    // when
    for (int i = 0; i < 10; ++i) {
        acc.Accumulate({"best metric", static_cast<metric::MetricResult::ValueType>(i)});
    }
    acc.Finalize();
    const auto got_value = acc.Get();

    // then
    EXPECT_DOUBLE_EQ(got_value, expected_value);
}

TEST(TestAverageAccumulator, TestAccumulateAfterReset) {
    // given
    AverageAccumulator acc;
    const metric::MetricResult given_metric_result{"given metric", 324};

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
    EXPECT_EQ(got_value, given_metric_result.metric_value);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
