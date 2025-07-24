#pragma once

#include "metric.hpp"

namespace analyser::metric::metric_impl {

struct ParametersCountMetric final : public IMetric {
public:
    std::string Name() const override { return ConstName(); }
    static std::string ConstName() { return "ParametersCount"; }

protected:
    MetricResult::ValueType CalculateImpl(const function::Function &function,
                                          const ast_extractor::ASTExtractor &ast_extractor) const override;
};

}  // namespace analyser::metric::metric_impl
