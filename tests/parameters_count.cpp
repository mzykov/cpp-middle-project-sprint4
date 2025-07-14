#include "metric_impl/parameters_count.hpp"
#include "analyse.hpp"
#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"

#include <gtest/gtest.h>

#include <tuple>

namespace analyser::metric::metric_impl::test {

// clang-format off
TEST(TestAnalyseFunctions, TestParametersCount) {
    // given
    const std::vector<std::string> given_file_names{
        "../files/many_parameters.py",
        "../files/simple.py"
    };
    MetricExtractor metric_extractor;
    metric_extractor.RegisterMetric(std::make_unique<ParametersCountMetric>());
    const std::vector<std::tuple<std::string, std::string, MetricResult::ValueType>> expected{
        { given_file_names[0], "__test_multiparameters__", 5 },
        { given_file_names[1], "test_simple",              0 },
    };

    // when
    const auto got = analyser::AnalyseFunctions(given_file_names, metric_extractor);

    // then
    EXPECT_EQ(got.size(), expected.size());

    for (size_t i = 0; i < expected.size(); ++i) {
        const auto &[got_function, got_metric_result] = got[i];
        EXPECT_EQ(got_function.file_name,            std::get<0>(expected[i]));
        EXPECT_EQ(got_function.function_name,        std::get<1>(expected[i]));
        EXPECT_EQ(got_metric_result[0].metric_value, std::get<2>(expected[i]));
    }
}
// clang-format on

}  // namespace analyser::metric::metric_impl::test
