#include "metric_accumulator.hpp"

namespace analyser::metric_accumulator {

void MetricsAccumulator::AccumulateNextFunctionResults(const metric::MetricResults &metric_results) const {
    for (auto &[_, acc_ptr] : accumulators_) {
        for (const auto metric_result : metric_results) {
            acc_ptr->Accumulate(metric_result);
        }
    }
}

void MetricsAccumulator::ResetAccumulators() {
    for (auto &[_, acc_ptr] : accumulators_) {
        acc_ptr->Reset();
    }
}

}  // namespace analyser::metric_accumulator
