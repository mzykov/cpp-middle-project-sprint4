#pragma once

#include <algorithm>
#include <print>
#include <ranges>

#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"

namespace analyser {

[[nodiscard]]
std::vector<std::pair<function::Function, metric::MetricResults>>
AnalyseFunctions(const std::vector<std::string> &file_names, const metric::MetricExtractor &metric_extractor);

}  // namespace analyser
