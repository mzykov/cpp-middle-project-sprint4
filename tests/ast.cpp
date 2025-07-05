#include <gtest/gtest.h>

#include "ast.hpp"

TEST(TestAnalyserAST, TestLinesIntervalContains) {
    // given
    auto outer_interval = analyser::ast::LinesInterval(22, 53);
    auto inner_interval = analyser::ast::LinesInterval(23, 53);
    // when
    bool got = outer_interval.Contains(inner_interval);
    // then
    EXPECT_TRUE(got);
}

TEST(TestAnalyserAST, TestLinesIntervalNotContains) {
    // given
    auto outer_interval = analyser::ast::LinesInterval(1, 5);
    auto inner_interval = analyser::ast::LinesInterval(6, 9);
    // when
    bool got = outer_interval.Contains(inner_interval);
    // then
    EXPECT_FALSE(got);
}

TEST(TestAnalyserAST, TestLinesIntervalCrosses) {
    // given
    auto outer_interval = analyser::ast::LinesInterval(1, 5);
    auto inner_interval = analyser::ast::LinesInterval(4, 9);
    // when
    bool got = outer_interval.Contains(inner_interval);
    // then
    EXPECT_FALSE(got);
}

TEST(TestAnalyserAST, TestExtractNodeName) {
    // given
    std::string_view given_ast = "(function_definition ";
    size_t expected_position = given_ast.length() - 1;
    size_t start_parsing_at = 1;
    auto expected_name = given_ast.substr(start_parsing_at, expected_position - start_parsing_at);
    // when
    auto got = analyser::ast::extractNodeName(given_ast, start_parsing_at);
    // then
    EXPECT_EQ(std::get<std::string>(got), expected_name);
    EXPECT_EQ(std::get<size_t>(got), expected_position);
}
