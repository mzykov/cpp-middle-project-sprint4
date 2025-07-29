#include "metric_accumulator.hpp"

namespace analyser::metric_accumulator {

void MetricAccumulator::AccumulateNextResults(const metric::MetricResults &metric_results) const {
    std::ranges::for_each(metric_results, [&](const auto &metric_result) {
        accumulators_.at(metric_result.metric_name)->Accumulate(metric_result);
    });
}

void MetricAccumulator::ResetAccumulators() {
    std::ranges::for_each(accumulators_, [](auto &p) { std::get<1>(p)->Reset(); });
}

}  // namespace analyser::metric_accumulator
