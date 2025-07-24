#include "metric_impl/metrics.hpp"

#include "analyse.hpp"
#include "file.hpp"
#include "function.hpp"

#include <gtest/gtest.h>

#include <tuple>

namespace analyser::metric::metric_impl::test {

class TestAnalyseFunctionsBase
    : public ::testing::TestWithParam<std::tuple<std::string, std::string, std::optional<std::string>, MetricResults>> {
public:
    TestAnalyseFunctionsBase() {
        metric_extractor.RegisterMetric(std::make_unique<CodeLinesCountMetric>());
        metric_extractor.RegisterMetric(std::make_unique<CyclomaticComplexityMetric>());
        metric_extractor.RegisterMetric(std::make_unique<ParametersCountMetric>());
    }
    static auto MetricTestCase(std::string &&file_name, std::string &&function_name, MetricResults &&results) {
        return std::make_tuple<std::string, std::string, std::optional<std::string>, MetricResults>(
            std::move(file_name), std::move(function_name), std::nullopt, std::move(results));
    }

protected:
    MetricExtractor metric_extractor{};
};

// clang-format off
TEST_P(TestAnalyseFunctionsBase, TestMetricsBase) {
    // given
    auto expected = GetParam();
    auto given_file_names = { std::get<0>(expected) };
    constexpr size_t expected_size = 1;

    // when
    const auto got = analyser::AnalyseFunctions(given_file_names, metric_extractor);

    // then
    EXPECT_EQ(got.size(), expected_size);

    // then
    const auto &[got_function, got_metric_result] = got[0];

    EXPECT_EQ(got_function.file_name,     std::get<0>(expected)) << "File names are not equal\n";
    EXPECT_EQ(got_function.function_name, std::get<1>(expected)) << "Function names are not equal\n";
    EXPECT_EQ(got_function.class_name,    std::get<2>(expected)) << "Class names are not equal\n";
    EXPECT_EQ(got_metric_result,          std::get<3>(expected)) << "Metric values are not equal\n";
}

INSTANTIATE_TEST_SUITE_P(
    SuiteAnalyseFunctionsBase, TestAnalyseFunctionsBase,
    ::testing::Values(
        TestAnalyseFunctionsBase::MetricTestCase("../files/comments.py",        "Func_comments",            {{CodeLinesCountMetric::ConstName(), 4},  {CyclomaticComplexityMetric::ConstName(), 0}, {ParametersCountMetric::ConstName(), 3}}),
        TestAnalyseFunctionsBase::MetricTestCase("../files/exceptions.py",      "Try_Exceptions",           {{CodeLinesCountMetric::ConstName(), 8},  {CyclomaticComplexityMetric::ConstName(), 4}, {ParametersCountMetric::ConstName(), 0}}),
        TestAnalyseFunctionsBase::MetricTestCase("../files/if.py",              "testIf",                   {{CodeLinesCountMetric::ConstName(), 4},  {CyclomaticComplexityMetric::ConstName(), 1}, {ParametersCountMetric::ConstName(), 1}}),
        TestAnalyseFunctionsBase::MetricTestCase("../files/loops.py",           "TestLoops",                {{CodeLinesCountMetric::ConstName(), 7},  {CyclomaticComplexityMetric::ConstName(), 3}, {ParametersCountMetric::ConstName(), 1}}),
        TestAnalyseFunctionsBase::MetricTestCase("../files/many_lines.py",      "testmultiline",            {{CodeLinesCountMetric::ConstName(), 12}, {CyclomaticComplexityMetric::ConstName(), 1}, {ParametersCountMetric::ConstName(), 0}}),
        TestAnalyseFunctionsBase::MetricTestCase("../files/many_parameters.py", "__test_multiparameters__", {{CodeLinesCountMetric::ConstName(), 2},  {CyclomaticComplexityMetric::ConstName(), 1}, {ParametersCountMetric::ConstName(), 5}}),
        TestAnalyseFunctionsBase::MetricTestCase("../files/match_case.py",      "test_Match_case",          {{CodeLinesCountMetric::ConstName(), 8},  {CyclomaticComplexityMetric::ConstName(), 4}, {ParametersCountMetric::ConstName(), 1}}),
        TestAnalyseFunctionsBase::MetricTestCase("../files/nested_if.py",       "Testnestedif",             {{CodeLinesCountMetric::ConstName(), 9},  {CyclomaticComplexityMetric::ConstName(), 5}, {ParametersCountMetric::ConstName(), 2}}),
        TestAnalyseFunctionsBase::MetricTestCase("../files/simple.py",          "test_simple",              {{CodeLinesCountMetric::ConstName(), 6},  {CyclomaticComplexityMetric::ConstName(), 1}, {ParametersCountMetric::ConstName(), 0}}),
        TestAnalyseFunctionsBase::MetricTestCase("../files/ternary.py",         "teSt_ternary",             {{CodeLinesCountMetric::ConstName(), 2},  {CyclomaticComplexityMetric::ConstName(), 4}, {ParametersCountMetric::ConstName(), 1}})
    )
);
// clang-format on
}  // namespace analyser::metric::metric_impl::test
