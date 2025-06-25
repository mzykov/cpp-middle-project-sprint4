#pragma once

#include "function.hpp"

namespace analyser::metric {

struct MetricResult {
    using ValueType = int;
    std::string metric_name;
    ValueType metric_value;
};

struct IMetric {
    virtual ~IMetric() = default;
    MetricResult Calculate(const function::Function &f, const ast_extractor::ASTExtractor &e) const {
        return MetricResult{
            .metric_name = Name(),
            .metric_value = CalculateImpl(f, e),
        };
    }

protected:
    virtual MetricResult::ValueType CalculateImpl(const function::Function &f,
                                                  const ast_extractor::ASTExtractor &e) const = 0;
    virtual std::string Name() const = 0;
};

using MetricResults = std::vector<MetricResult>;

class MetricExtractor {
public:
    void RegisterMetric(std::unique_ptr<IMetric> &&metric);
    MetricResults ProcessOneFunction(const function::Function &func) const;

private:
    std::vector<std::unique_ptr<IMetric>> metrics_;
    ast_extractor::ASTExtractor extractor_;
};

}  // namespace analyser::metric
