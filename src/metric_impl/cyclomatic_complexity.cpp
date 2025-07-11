#include <queue>
#include <unordered_set>

#include "metric_impl/cyclomatic_complexity.hpp"

namespace analyser::metric::metric_impl {

MetricResult::ValueType
CyclomaticComplexityMetric::CalculateImpl(const function::Function &function,
                                          const ast_extractor::ASTExtractor &ast_extractor) const {
    MetricResult::ValueType res = 0;
    std::queue<std::pair<std::string, size_t>> q;
    std::unordered_set<ast::Rect> already;

    constexpr size_t start_parsing_at = 0;
    q.emplace(function.ast, start_parsing_at);

    while (!q.empty()) {
        const auto [ast, continue_parsing_at] = q.front();
        q.pop();

        std::ranges::for_each(ast_extractor.CyclomaticKeywords(), [&](std::string_view kw) {
            const auto data = ast_extractor.ExtractASTFragment(ast, kw, continue_parsing_at);
            if (data) {
                const auto [nested_ast, further_parsing_at] = *data;
                q.emplace(ast, further_parsing_at);
                q.emplace(nested_ast, start_parsing_at + kw.length());

                const auto rect_data = ast_extractor.ExtractRect(nested_ast, start_parsing_at);
                if (rect_data) {
                    const auto [rect, _] = *rect_data;

                    if (!already.contains(rect)) {
                        already.insert(rect);
                        ++res;
                    }
                }
            }
        });
    }

    return res;
}

}  // namespace analyser::metric::metric_impl
