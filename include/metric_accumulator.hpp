#pragma once

#include <format>
#include <unordered_map>

#include "metric.hpp"

namespace analyser::metric_accumulator {

struct IAccumulator {
    virtual void Accumulate(const metric::MetricResult &metric_result) = 0;
    virtual void Finalize() = 0;
    virtual void Reset() = 0;
    virtual ~IAccumulator() = default;

protected:
    bool is_finalized_ = false;
};

struct MetricAccumulator {
    template <typename Accumulator>
    void RegisterAccumulator(const std::string &metric_name, const std::shared_ptr<Accumulator> acc) {
        accumulators_[metric_name] = acc;
    }

    template <typename Accumulator>
    const Accumulator &GetFinalizedAccumulator(const std::string &metric_name) const {
        auto acc = dynamic_cast<Accumulator *>(accumulators_.at(metric_name).get());
        if (acc) {
            acc->Finalize();
            return *acc;
        } else {
            throw std::runtime_error("No accumulator found for metric " + metric_name);
        }
    }

    void AccumulateNextResults(const metric::MetricResults &metric_results) const;
    void ResetAccumulators();

private:
    std::unordered_map<std::string, std::shared_ptr<IAccumulator>> accumulators_;
};

}  // namespace analyser::metric_accumulator
