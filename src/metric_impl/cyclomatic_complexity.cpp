#include "metric_impl/cyclomatic_complexity.hpp"

namespace analyser::metric::metric_impl {

MetricResult::ValueType CyclomaticComplexityMetric::CalculateImpl(const function::Function &f,
                                                                  const ast_extractor::ASTExtractor &e) const {
    return 0;
}

}  // namespace analyser::metric::metric_impl
