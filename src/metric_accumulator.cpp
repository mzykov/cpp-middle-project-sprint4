#include "metric_accumulator.hpp"

namespace analyser::metric_accumulator {

void MetricAccumulator::AccumulateNextResults(const metric::MetricResults &metric_results) const {
    std::ranges::for_each(accumulators_, [&metric_results](auto &p) {
        const auto &[metric_name, acc] = p;
        std::ranges::for_each(metric_results, [&metric_name, &acc](const auto &metric_result) {
            if (metric_name == metric_result.metric_name) {
                acc->Accumulate(metric_result);
            }
        });
    });
}

void MetricAccumulator::ResetAccumulators() {
    std::ranges::for_each(accumulators_, [](auto &p) { std::get<1>(p)->Reset(); });
}

}  // namespace analyser::metric_accumulator
