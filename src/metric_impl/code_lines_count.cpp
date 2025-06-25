#include "metric_impl/code_lines_count.hpp"

namespace analyser::metric::metric_impl {

MetricResult::ValueType CodeLinesCountMetric::CalculateImpl(const function::Function &f,
                                                            const ast_extractor::ASTExtractor &e) const {
    return 0;
}

}  // namespace analyser::metric::metric_impl
