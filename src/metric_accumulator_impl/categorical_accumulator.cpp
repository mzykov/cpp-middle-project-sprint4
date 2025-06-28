#include "metric_accumulator_impl/categorical_accumulator.hpp"

namespace analyser::metric_accumulator::metric_accumulator_impl {

void CategoricalAccumulator::Accumulate(const metric::MetricResult &metric_result) {}

void CategoricalAccumulator::Finalize() {}

void CategoricalAccumulator::Reset() {
    categories_freq_.clear();
    is_finalized_ = false;
}

const std::unordered_map<std::string, int> &CategoricalAccumulator::Get() const { return categories_freq_; }

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
