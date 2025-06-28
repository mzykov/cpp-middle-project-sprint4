#pragma once

#include "metric_accumulator.hpp"

namespace analyser::metric_accumulator::metric_accumulator_impl {

struct AverageAccumulator final : public IAccumulator {
    void Accumulate(const metric::MetricResult &metric_result) override;

    void Finalize() override;

    void Reset();

    double Get() const;

private:
    int sum_ = 0;
    int count_ = 0;
    double average_ = 0;
};

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
