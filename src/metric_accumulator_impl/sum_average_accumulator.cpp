#include "metric_accumulator_impl/sum_average_accumulator.hpp"

namespace analyser::metric_accumulator::metric_accumulator_impl {

void SumAverageAccumulator::Accumulate(const metric::MetricResult &metric_result) {}

void SumAverageAccumulator::Finalize() {}

void SumAverageAccumulator::Reset() {
    sum_ = 0;
    count_ = 0;
    average_ = 0.0;
    is_finalized_ = false;
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
