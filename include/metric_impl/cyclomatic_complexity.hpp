#pragma once

#include "metric.hpp"

namespace analyser::metric::metric_impl {

struct CyclomaticComplexityMetric final : public IMetric {
protected:
    MetricResult::ValueType CalculateImpl(const function::Function &function,
                                          const ast_extractor::ASTExtractor &ast_extractor) const override;

    std::string Name() const override { return "CyclomaticComplexity"; }
};

}  // namespace analyser::metric::metric_impl
