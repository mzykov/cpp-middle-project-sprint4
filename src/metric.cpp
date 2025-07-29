#include "metric.hpp"

namespace analyser::metric {

void MetricExtractor::RegisterMetric(std::unique_ptr<IMetric> &&metric) { metrics_.emplace_back(std::move(metric)); }

MetricResults MetricExtractor::ProcessOneFunction(const function::Function &function,
                                                  const ast_extractor::ASTExtractor &ast_extractor) const {
    return metrics_ | std::views::transform([&](const auto &metric_ptr) {
               return metric_ptr->Calculate(function, ast_extractor);
           }) |
           std::ranges::to<MetricResults>();
}

}  // namespace analyser::metric
