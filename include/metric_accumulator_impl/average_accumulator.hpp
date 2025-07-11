#pragma once

#include "metric_accumulator.hpp"

namespace analyser::metric_accumulator::metric_accumulator_impl {

struct AverageAccumulator final : public IAccumulator {
    void Accumulate(const metric::MetricResult &metric_result) override;
    void Finalize() override;
    void Reset() override;

    double Get() const;

private:
    int sum_ = 0;
    int count_ = 0;
    double average_ = 0;
};

}  // namespace analyser::metric_accumulator::metric_accumulator_impl

namespace std {

using namespace analyser::metric_accumulator::metric_accumulator_impl;

template <>
struct formatter<AverageAccumulator> {
    template <typename FormatContext>
    auto format(const AverageAccumulator &acc, FormatContext &fc) const {
        format_to(fc.out(), "{:.3}", acc.Get());
        return fc.out();
    }
    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

}  // namespace std
