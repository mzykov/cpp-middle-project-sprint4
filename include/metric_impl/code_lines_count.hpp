#pragma once

#include "metric.hpp"

namespace analyser::metric::metric_impl {

struct CodeLinesCountMetric final : public IMetric {
public:
    std::string Name() const override { return "CodeLinesCount"; }

protected:
    MetricResult::ValueType CalculateImpl(const function::Function &function,
                                          const ast_extractor::ASTExtractor &ast_extractor) const override;
};

}  // namespace analyser::metric::metric_impl
