#include "metric_impl/parameters_count.hpp"

namespace analyser::metric::metric_impl {

MetricResult::ValueType ParametersCountMetric::CalculateImpl(const function::Function &f,
                                                             const ast_extractor::ASTExtractor &e) const {
    const auto data = e.ExtractParametersASTFragment(f.ast);

    if (data) {
        const auto [parameters_ast, _] = *data;
        return e.CountFirstLevelASTNodes(parameters_ast);
    } else {
        return 0;
    }
}

}  // namespace analyser::metric::metric_impl
