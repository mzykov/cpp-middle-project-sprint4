#include "metric_accumulator_impl/categorical_accumulator.hpp"

namespace analyser::metric_accumulator::metric_accumulator_impl {

void CategoricalAccumulator::Accumulate(const metric::MetricResult &metric_result) {
    if (is_finalized_)
        return;
    categories_freq_[metric_result.metric_name] += metric_result.metric_value;
}

void CategoricalAccumulator::Finalize() { is_finalized_ = true; }

void CategoricalAccumulator::Reset() {
    categories_freq_.clear();
    is_finalized_ = false;
}

const std::unordered_map<std::string, int> &CategoricalAccumulator::Get() const { return categories_freq_; }

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
