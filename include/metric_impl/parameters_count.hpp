#pragma once

#include "metric.hpp"

namespace analyser::metric::metric_impl {

struct ParametersCountMetric final : public IMetric {
public:
    std::string Name() const override { return "ParametersCount"; }

protected:
    MetricResult::ValueType CalculateImpl(const function::Function &f,
                                          const ast_extractor::ASTExtractor &e) const override;
};

}  // namespace analyser::metric::metric_impl
