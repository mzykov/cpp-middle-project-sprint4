#include <gtest/gtest.h>

#include "ast.hpp"

TEST(TestAnalyserAST, TestLinesIntervalContains) {
    // given
    constexpr auto outer_interval = analyser::ast::LinesInterval(22, 53);
    constexpr auto inner_interval = analyser::ast::LinesInterval(23, 53);
    // when
    constexpr bool got = outer_interval.Contains(inner_interval);
    // then
    EXPECT_TRUE(got);
}

TEST(TestAnalyserAST, TestLinesIntervalNotContains) {
    // given
    constexpr auto outer_interval = analyser::ast::LinesInterval(1, 5);
    constexpr auto inner_interval = analyser::ast::LinesInterval(6, 9);
    // when
    constexpr bool got = outer_interval.Contains(inner_interval);
    // then
    EXPECT_FALSE(got);
}

TEST(TestAnalyserAST, TestLinesIntervalCrosses) {
    // given
    constexpr auto outer_interval = analyser::ast::LinesInterval(1, 5);
    constexpr auto inner_interval = analyser::ast::LinesInterval(4, 9);
    // when
    constexpr bool got = outer_interval.Contains(inner_interval);
    // then
    EXPECT_FALSE(got);
}
