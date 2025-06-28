#include "metric_accumulator.hpp"

namespace analyser::metric_accumulator {

void MetricsAccumulator::AccumulateNextFunctionResults(const metric::MetricResults &metric_results) const {
    // здесь ваш код
}

void MetricsAccumulator::ResetAccumulators() {
    for (auto &[_, acc_ptr] : accumulators_) {
        acc_ptr->Reset();
    }
}

}  // namespace analyser::metric_accumulator
