#include "metric_impl/cyclomatic_complexity.hpp"

#include "analyse.hpp"
#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"

#include <gtest/gtest.h>

#include <tuple>

namespace analyser::metric::metric_impl::test {

// clang-format off
TEST(TestAnalyseFunctions, TestCyclomaticComplexity) {
    // given
    const std::vector<std::string> given_file_names{
        "../files/exceptions.py",
        "../files/if.py",
        "../files/loops.py",
        "../files/match_case.py",
        "../files/nested_if.py",
        "../files/ternary.py",
    };
    MetricExtractor metric_extractor;
    metric_extractor.RegisterMetric(std::make_unique<CyclomaticComplexityMetric>());
    const std::vector<std::tuple<std::string, std::string, MetricResult::ValueType>> expected{
        { given_file_names[0], "Try_Exceptions",  4 },
        { given_file_names[1], "testIf",          1 },
        { given_file_names[2], "TestLoops",       3 },
        { given_file_names[3], "test_Match_case", 4 },
        { given_file_names[4], "Testnestedif",    5 },
        { given_file_names[5], "teSt_ternary",    4 },
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
