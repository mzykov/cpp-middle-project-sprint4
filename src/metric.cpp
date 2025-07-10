#include "metric.hpp"

namespace analyser::metric {

void MetricExtractor::RegisterMetric(std::unique_ptr<IMetric> &&metric) { metrics_.emplace_back(std::move(metric)); }

MetricResults MetricExtractor::ProcessOneFunction(const function::Function &function) const {
    MetricResults res;
    std::ranges::for_each(
        metrics_, [&](const auto &metric_ptr) { res.emplace_back(metric_ptr->Calculate(function, extractor_)); });
    return res;
}

}  // namespace analyser::metric
