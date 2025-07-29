#include <gtest/gtest.h>

#include "ast.hpp"
#include <cassert>

TEST(TestAnalyserAST, TestLinesIntervalContains) {
    // given
    constexpr auto outer_interval = analyser::ast::LinesInterval(22, 53);
    constexpr auto inner_interval = analyser::ast::LinesInterval(23, 53);
    // when
    constexpr bool got = outer_interval.Contains(inner_interval);
    // then
    static_assert(got == true, "Line interval {22,53} does not contain {23,53}");
    EXPECT_TRUE(got);
}

TEST(TestAnalyserAST, TestLinesIntervalContainsItself) {
    // given
    constexpr auto outer_interval = analyser::ast::LinesInterval(37, 59);
    constexpr auto inner_interval = outer_interval;
    // when
    constexpr bool got = outer_interval.Contains(inner_interval);
    // then
    static_assert(got == true, "Line interval {37,59} does not contain itself");
    EXPECT_TRUE(got);
}

TEST(TestAnalyserAST, TestLinesIntervalNotContains) {
    // given
    constexpr auto outer_interval = analyser::ast::LinesInterval(1, 5);
    constexpr auto inner_interval = analyser::ast::LinesInterval(6, 9);
    // when
    constexpr bool got = outer_interval.Contains(inner_interval);
    // then
    static_assert(got == false, "Line interval {1,5} must not contain {6,9}");
    EXPECT_FALSE(got);
}

TEST(TestAnalyserAST, TestLinesIntervalCrosses) {
    // given
    constexpr auto outer_interval = analyser::ast::LinesInterval(1, 5);
    constexpr auto inner_interval = analyser::ast::LinesInterval(4, 9);
    // when
    constexpr bool got = outer_interval.Contains(inner_interval);
    // then
    static_assert(got == false, "Line interval {1,5} must not contain {4,9}");
    EXPECT_FALSE(got);
}
