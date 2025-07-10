#pragma once

#include "metric_accumulator.hpp"

namespace analyser::metric_accumulator::metric_accumulator_impl {

struct SumAverageAccumulator final : public IAccumulator {
    struct SumAverage {
        int sum;
        double average;
        auto operator<=>(const SumAverage &) const = default;
    };

    void Accumulate(const metric::MetricResult &metric_result) override;
    virtual void Finalize() override;
    virtual void Reset() override;

    SumAverage Get() const;

private:
    int sum_ = 0;
    int count_ = 0;
    double average_ = 0;
};

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
