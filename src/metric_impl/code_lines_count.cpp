#include <queue>
#include <stack>
#include <unordered_set>

#include "metric_impl/code_lines_count.hpp"

namespace analyser::metric::metric_impl {

MetricResult::ValueType CodeLinesCountMetric::CalculateImpl(const function::Function &function,
                                                            const ast_extractor::ASTExtractor &ast_extractor) const {
    std::string_view function_ast = function.ast;

    std::unordered_set<size_t> lines;
    auto comment_lines = ast_extractor.ExtractAllCommentLineNumbers(function_ast);

    std::stack<ast::LinesInterval> s;

    constexpr size_t start_parsing_at = 0;
    std::queue<std::pair<std::string_view, size_t>> q;

    q.emplace(function_ast, start_parsing_at);

    while (!q.empty()) {
        const auto [ast_fragment, continue_parsing_at] = q.front();
        q.pop();

        const auto rect_data = ast_extractor.ExtractRect(ast_fragment, continue_parsing_at);

        if (rect_data) {
            const auto [rect, further_parsing_at] = *rect_data;
            q.emplace(ast_fragment, further_parsing_at);

            const auto current_interval = ast::LinesInterval{rect};

            while (!s.empty()) {
                const auto spanning_interval = s.top();

                if (spanning_interval.Contains(current_interval)) {
                    break;
                } else {
                    lines.insert(spanning_interval.start_line);
                    lines.insert(spanning_interval.end_line);
                    s.pop();
                }
            }

            const auto found_it = std::find(comment_lines.begin(), comment_lines.end(), current_interval.start_line);

            if (current_interval.IsOneLine() && found_it != comment_lines.end()) {
                comment_lines.erase(found_it);
            } else {
                s.push(current_interval);
            }
        } else {
            while (!s.empty()) {
                const auto interval = s.top();
                lines.insert(interval.start_line);
                lines.insert(interval.end_line);
                s.pop();
            }
        }
    }

    return static_cast<MetricResult::ValueType>(lines.size());
}

}  // namespace analyser::metric::metric_impl
