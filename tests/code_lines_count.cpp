#include "metric_impl/code_lines_count.hpp"
#include "analyse.hpp"
#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"

#include <gtest/gtest.h>

#include <tuple>

namespace analyser::metric::metric_impl::test {

// clang-format off
TEST(TestAnalyseFunctions, TestCodeLinesCount) {
    // given
    const std::vector<std::string> given_file_names{
        "../files/sample.py",
        "../files/comments.py",
        "../files/many_lines.py",
        "../files/simple.py"
    };
    MetricExtractor metric_extractor;
    metric_extractor.RegisterMetric(std::make_unique<CodeLinesCountMetric>());
    const std::vector<std::tuple<std::string, std::string, std::optional<std::string>, MetricResult::ValueType>> expected{
        { given_file_names[0], "__init__",      "AdvancedProcessor", 3  },
        { given_file_names[0], "process",       "AdvancedProcessor", 8  },
        { given_file_names[0], "__call__",      "AdvancedProcessor", 5  },
        { given_file_names[0], "nested_def",    "AdvancedProcessor", 3  },
        { given_file_names[0], "lambda_demo",   std::nullopt,        22 },
        { given_file_names[1], "Func_comments", std::nullopt,        4  },
        { given_file_names[2], "testmultiline", std::nullopt,        12 },
        { given_file_names[3], "test_simple",   std::nullopt,        6  },
    };

    // when
    const auto got = analyser::AnalyseFunctions(given_file_names, metric_extractor);

    // then
    EXPECT_EQ(got.size(), expected.size());

    for (size_t i = 0; i < expected.size(); ++i) {
        const auto &[got_function, got_metric_result] = got[i];
        EXPECT_EQ(got_function.file_name,            std::get<0>(expected[i]));
        EXPECT_EQ(got_function.function_name,        std::get<1>(expected[i]));
        EXPECT_EQ(got_function.class_name,           std::get<2>(expected[i]));
        EXPECT_EQ(got_metric_result[0].metric_value, std::get<3>(expected[i]));
    }
}
// clang-format on

}  // namespace analyser::metric::metric_impl::test
