#pragma once

#include "metric.hpp"

namespace analyser::metric::metric_impl {

struct CyclomaticComplexityMetric final : public IMetric {
protected:
    MetricResult::ValueType CalculateImpl(const function::Function &f,
                                          const ast_extractor::ASTExtractor &e) const override;
    std::string Name() const override { return "CyclomaticComplexity"; }

private:
    MetricResult::ValueType CalculateImplForAST(const std::string &ast, const ast_extractor::ASTExtractor &e) const;
};

}  // namespace analyser::metric::metric_impl
