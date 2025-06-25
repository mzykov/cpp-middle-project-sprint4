#include "metric_impl/parameters_count.hpp"

namespace analyser::metric::metric_impl {

MetricResult::ValueType ParametersCountMetric::CalculateImpl(const function::Function &f,
                                                             const ast_extractor::ASTExtractor &e) const {
    return 0;
    //    const std::string marker = "(parameters";
    //    const auto [parameters_ast, _] = f.parser->extractSubtree(f.ast, marker, 0);
    //    return f.parser->countFirstLevelSubNodes(parameters_ast);
}

}  // namespace analyser::metric::metric_impl
