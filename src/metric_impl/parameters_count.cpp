#include "metric_impl/parameters_count.hpp"

namespace analyser::metric::metric_impl {

MetricResult::ValueType ParametersCountMetric::CalculateImpl(const function::Function &function,
                                                             const ast_extractor::ASTExtractor &ast_extractor) const {
    const auto data = ast_extractor.ExtractParametersASTFragment(function.ast);

    if (data) {
        const auto [parameters_ast, _] = *data;
        return ast_extractor.CountFirstLevelASTNodes(parameters_ast);
    } else {
        return 0;
    }
}

}  // namespace analyser::metric::metric_impl
