#include "metric_accumulator_impl/average_accumulator.hpp"

namespace analyser::metric_accumulator::metric_accumulator_impl {

void AverageAccumulator::Accumulate(const metric::MetricResult &metric_result) {
    if (is_finalized_)
        throw std::runtime_error("Using finalized accumulator is not allowed");
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

double AverageAccumulator::Get() const {
    if (is_finalized_)
        return average_;
    else
        throw std::runtime_error("Accumulator is not finalized");
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
