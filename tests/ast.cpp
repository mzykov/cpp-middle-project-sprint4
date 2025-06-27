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
