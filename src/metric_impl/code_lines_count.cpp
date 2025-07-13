#include <stack>
#include <unordered_set>

#include "metric_impl/code_lines_count.hpp"

namespace analyser::metric::metric_impl {

MetricResult::ValueType CodeLinesCountMetric::CalculateImpl(const function::Function &function,
                                                            const ast_extractor::ASTExtractor &ast_extractor) const {
    std::unordered_set<size_t> res;
    auto comments = ast_extractor.ExtractAllCommentLineNumbers(function.ast);

    constexpr size_t start_parsing_at = 0;
    const auto function_data = ast_extractor.ExtractRect(function.ast, start_parsing_at);

    if (!function_data) {
        return static_cast<MetricResult::ValueType>(res.size());
    }

    auto [function_rect, continue_parsing_at] = *function_data;

    std::stack<ast::LinesInterval> s;
    s.push(ast::LinesInterval{function_rect});

    while (!s.empty()) {
        const auto next_data = ast_extractor.ExtractRect(function.ast, continue_parsing_at);

        if (!next_data) {
            while (!s.empty()) {
                res.insert(s.top().start_line);
                s.pop();
            }
            break;
        }

        const auto [next_rect, further_parsing_at] = *next_data;
        continue_parsing_at = further_parsing_at;

        const auto next_interval = ast::LinesInterval{next_rect};

        while (!s.empty()) {
            const auto spanning_interval = s.top();

            if (spanning_interval.Contains(next_interval)) {
                break;
            } else {
                res.insert(spanning_interval.start_line);
                s.pop();
            }
        }

        if (next_interval.IsOneLine() && comments.contains(next_interval.start_line)) {
            comments.erase(next_interval.start_line);
        } else {
            s.push(next_interval);
        }
    }

    return static_cast<MetricResult::ValueType>(res.size());
}

}  // namespace analyser::metric::metric_impl
