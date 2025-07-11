#include "metric_accumulator_impl/average_accumulator.hpp"

#include <print>

namespace analyser::metric_accumulator::metric_accumulator_impl {

void AverageAccumulator::Accumulate(const metric::MetricResult &metric_result) {
    if (is_finalized_)
        return;
    sum_ += metric_result.metric_value;
    ++count_;
}

void AverageAccumulator::Finalize() {
    if (count_ > 0)
        average_ = static_cast<double>(sum_) / static_cast<double>(count_);
    is_finalized_ = true;
}

void AverageAccumulator::Reset() {
    sum_ = 0;
    count_ = 0;
    average_ = 0.0;
    is_finalized_ = false;
}

double AverageAccumulator::Get() const { return average_; }

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
