#pragma once

#include "metric.hpp"

namespace analyser::metric::metric_impl {

struct ParametersCountMetric final : public IMetric {
protected:
    MetricResult::ValueType CalculateImpl(const function::Function &f,
                                          const ast_extractor::ASTExtractor &e) const override;
    std::string Name() const override { return "ParametersCount"; }
};

}  // namespace analyser::metric::metric_impl
