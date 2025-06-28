#include "metric_accumulator_impl/average_accumulator.hpp"

namespace analyser::metric_accumulator::metric_accumulator_impl {

void AverageAccumulator::Accumulate(const metric::MetricResult &metric_result) {}

void AverageAccumulator::Finalize() {}

void AverageAccumulator::Reset() {
    sum_ = 0;
    count_ = 0;
    average_ = 0.0;
    is_finalized_ = false;
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
