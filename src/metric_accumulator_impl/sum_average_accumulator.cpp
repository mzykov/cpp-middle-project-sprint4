#include "metric_accumulator_impl/sum_average_accumulator.hpp"

namespace analyser::metric_accumulator::metric_accumulator_impl {

void SumAverageAccumulator::Accumulate(const metric::MetricResult &metric_result) {
    if (is_finalized_)
        return;
    sum_ += metric_result.metric_value;
    ++count_;
}

void SumAverageAccumulator::Finalize() {
    if (count_ > 0)
        average_ = static_cast<double>(sum_) / static_cast<double>(count_);
    is_finalized_ = true;
}

void SumAverageAccumulator::Reset() {
    sum_ = 0;
    count_ = 0;
    average_ = 0.0;
    is_finalized_ = false;
}

SumAverageAccumulator::SumAverage SumAverageAccumulator::Get() const { return {sum_, average_}; }

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
