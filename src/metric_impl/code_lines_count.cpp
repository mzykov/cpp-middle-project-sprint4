#include <stack>
#include <unordered_set>

#include "metric_impl/code_lines_count.hpp"

namespace analyser::metric::metric_impl {

MetricResult::ValueType CodeLinesCountMetric::CalculateImpl(const function::Function &function,
                                                            const ast_extractor::ASTExtractor &ast_extractor) const {
    std::unordered_set<size_t> res;

    auto comments = ast_extractor.ExtractAllCommentLineNumbers(function.ast);

    auto func_data = ast_extractor.ExtractRect(function.ast, 0);
    if (!func_data) {
        return static_cast<MetricResult::ValueType>(res.size());
    }
    auto [func_rect, continue_parsing_at] = *func_data;

    std::stack<ast::LinesInterval> s;
    s.push(ast::LinesInterval{func_rect});

    while (!s.empty()) {
        auto next_data = ast_extractor.ExtractRect(function.ast, continue_parsing_at);

        if (!next_data) {
            while (!s.empty()) {
                res.insert(s.top().start_line);
                s.pop();
            }
            break;
        }

        continue_parsing_at = std::get<size_t>(*next_data);

        auto spanning_interval = s.top();
        const auto next_interval = ast::LinesInterval{std::get<ast::Rect>(*next_data)};

        while (!s.empty() && !spanning_interval.Contains(next_interval)) {
            res.insert(spanning_interval.start_line);
            s.pop();
            if (!s.empty()) {
                spanning_interval = s.top();
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
