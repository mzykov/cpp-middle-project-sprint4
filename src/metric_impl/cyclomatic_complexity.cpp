#include "metric_impl/cyclomatic_complexity.hpp"

namespace analyser::metric::metric_impl {

MetricResult::ValueType CyclomaticComplexityMetric::CalculateImpl(const function::Function &f,
                                                                  const ast_extractor::ASTExtractor &e) const {
    return CalculateImplForAST(f.ast, e);
}

MetricResult::ValueType CyclomaticComplexityMetric::CalculateImplForAST(const std::string &ast,
                                                                        const ast_extractor::ASTExtractor &e) const {
    MetricResult::ValueType res = 0;
    // res += CalculateIfElifElseComplexity(ast, e);
    return res;
}

}  // namespace analyser::metric::metric_impl
