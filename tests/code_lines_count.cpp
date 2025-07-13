#include "metric_impl/code_lines_count.hpp"
#include "analyse.hpp"
#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"

#include <gtest/gtest.h>

#include <tuple>

namespace analyser::metric::metric_impl::test {

TEST(TestAnalyseFunctions, TestCodeLinesCount) {
    // given
    const std::vector<std::string> given_file_names{"../files/sample.py"};
    MetricExtractor metric_extractor;
    metric_extractor.RegisterMetric(std::make_unique<CodeLinesCountMetric>());
    const std::vector<std::tuple<std::string, std::optional<std::string>, MetricResult::ValueType>> expected{
        {"__init__", "AdvancedProcessor", 3},
        {"process", "AdvancedProcessor", 8},
        {"__call__", "AdvancedProcessor", 5},
        {"nested_def", "AdvancedProcessor", 3},
        {"lambda_demo", std::nullopt, 22}};

    // when
    const auto got = analyser::AnalyseFunctions(given_file_names, metric_extractor);

    // then
    EXPECT_EQ(got.size(), expected.size());

    for (size_t i = 0; i < expected.size(); ++i) {
        const auto &[got_function, got_metric_result] = got[i];
        EXPECT_EQ(got_function.function_name, std::get<std::string>(expected[i]));
        EXPECT_EQ(got_function.class_name, std::get<std::optional<std::string>>(expected[i]));
        EXPECT_EQ(got_metric_result[0].metric_value, std::get<MetricResult::ValueType>(expected[i]));
    }
}

}  // namespace analyser::metric::metric_impl::test
