#include "metric_impl/metrics.hpp"

#include "analyse.hpp"
#include "file.hpp"
#include "function.hpp"

#include <gtest/gtest.h>

#include <tuple>

namespace analyser::metric::metric_impl::test {

// clang-format off
TEST(TestAnalyseFunctions, TestMetrics) {
    // given
    const std::vector<std::string> given_file_names{
        "../files/comments.py",
        "../files/exceptions.py",
        "../files/if.py",
        "../files/loops.py",
        "../files/many_lines.py",
        "../files/many_parameters.py",
        "../files/match_case.py",
        "../files/nested_if.py",
        "../files/sample.py",
        "../files/simple.py",
        "../files/ternary.py",
    };

    MetricExtractor metric_extractor;
    auto CodeLinesCountMetricName       = metric_extractor.RegisterMetric(std::make_unique<CodeLinesCountMetric>());
    auto CyclomaticComplexityMetricName = metric_extractor.RegisterMetric(std::make_unique<CyclomaticComplexityMetric>());
    auto ParametersCountMetricName      = metric_extractor.RegisterMetric(std::make_unique<ParametersCountMetric>());

    const std::vector<std::tuple<std::string, std::string, std::optional<std::string>, MetricResults>> expected{
        { given_file_names[0],  "Func_comments",            std::nullopt,        {{CodeLinesCountMetricName, 4},  {CyclomaticComplexityMetricName, 0}, {ParametersCountMetricName, 3}} },
        { given_file_names[1],  "Try_Exceptions",           std::nullopt,        {{CodeLinesCountMetricName, 8},  {CyclomaticComplexityMetricName, 4}, {ParametersCountMetricName, 0}} },
        { given_file_names[2],  "testIf",                   std::nullopt,        {{CodeLinesCountMetricName, 4},  {CyclomaticComplexityMetricName, 1}, {ParametersCountMetricName, 1}} },
        { given_file_names[3],  "TestLoops",                std::nullopt,        {{CodeLinesCountMetricName, 7},  {CyclomaticComplexityMetricName, 3}, {ParametersCountMetricName, 1}} },
        { given_file_names[4],  "testmultiline",            std::nullopt,        {{CodeLinesCountMetricName, 12}, {CyclomaticComplexityMetricName, 1}, {ParametersCountMetricName, 0}} },
        { given_file_names[5],  "__test_multiparameters__", std::nullopt,        {{CodeLinesCountMetricName, 2},  {CyclomaticComplexityMetricName, 1}, {ParametersCountMetricName, 5}} },
        { given_file_names[6],  "test_Match_case",          std::nullopt,        {{CodeLinesCountMetricName, 8},  {CyclomaticComplexityMetricName, 4}, {ParametersCountMetricName, 1}} },
        { given_file_names[7],  "Testnestedif",             std::nullopt,        {{CodeLinesCountMetricName, 9},  {CyclomaticComplexityMetricName, 5}, {ParametersCountMetricName, 2}} },
        { given_file_names[8],  "__init__",                 "AdvancedProcessor", {{CodeLinesCountMetricName, 3},  {CyclomaticComplexityMetricName, 0}, {ParametersCountMetricName, 3}} },
        { given_file_names[8],  "process",                  "AdvancedProcessor", {{CodeLinesCountMetricName, 8},  {CyclomaticComplexityMetricName, 0}, {ParametersCountMetricName, 2}} },
        { given_file_names[8],  "__call__",                 "AdvancedProcessor", {{CodeLinesCountMetricName, 5},  {CyclomaticComplexityMetricName, 0}, {ParametersCountMetricName, 2}} },
        { given_file_names[8],  "nested_def",               "AdvancedProcessor", {{CodeLinesCountMetricName, 3},  {CyclomaticComplexityMetricName, 0}, {ParametersCountMetricName, 0}} },
        { given_file_names[8],  "lambda_demo",              std::nullopt,        {{CodeLinesCountMetricName, 22}, {CyclomaticComplexityMetricName, 9}, {ParametersCountMetricName, 0}} },
        { given_file_names[9],  "test_simple",              std::nullopt,        {{CodeLinesCountMetricName, 6},  {CyclomaticComplexityMetricName, 1}, {ParametersCountMetricName, 0}} },
        { given_file_names[10], "teSt_ternary",             std::nullopt,        {{CodeLinesCountMetricName, 2},  {CyclomaticComplexityMetricName, 4}, {ParametersCountMetricName, 1}} },
    };

    // when
    const auto got = analyser::AnalyseFunctions(given_file_names, metric_extractor);

    // then
    EXPECT_EQ(got.size(), expected.size());

    for (size_t i = 0; i < expected.size(); ++i) {
        const auto &[got_function, got_metric_result] = got[i];
        EXPECT_EQ(got_function.file_name,     std::get<0>(expected[i])) << "Iter [" << i << "] file names are not equal\n";
        EXPECT_EQ(got_function.function_name, std::get<1>(expected[i])) << "Iter [" << i << "] function names are not equal\n";
        EXPECT_EQ(got_function.class_name,    std::get<2>(expected[i])) << "Iter [" << i << "] class names are not equal\n";
        EXPECT_EQ(got_metric_result,          std::get<3>(expected[i])) << "Iter [" << i << "] metric values are not equal\n";
    }
}
// clang-format on

}  // namespace analyser::metric::metric_impl::test
