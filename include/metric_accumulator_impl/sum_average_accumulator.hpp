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

namespace std {

using namespace analyser::metric_accumulator::metric_accumulator_impl;

template <>
struct formatter<SumAverageAccumulator> {
    template <typename FormatContext>
    auto format(const SumAverageAccumulator &acc, FormatContext &fc) const {
        auto val = acc.Get();
        format_to(fc.out(), "sum = {}, avg = {:.3}", val.sum, val.average);
        return fc.out();
    }
    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

}  // namespace std
