#include "metric_accumulator_impl/categorical_accumulator.hpp"

namespace analyser::metric_accumulator::metric_accumulator_impl {

void CategoricalAccumulator::Reset() {
    categories_freq_.clear();
    is_finalized_ = false;
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
