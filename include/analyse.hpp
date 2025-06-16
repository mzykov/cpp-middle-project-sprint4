#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"

namespace analyser {

namespace rv = std::ranges::views;
namespace rs = std::ranges;

auto analyseFunctionsForFile(const std::string &filename) {
    file::File file(filename);
    function::FunctionExtractor extractor;
    return extractor.Get(file);
}

auto AnalyseFunctions(const std::vector<std::string> &files,
                      const analyser::metric::MetricExtractor &metric_extractor) {
    std::vector<std::pair<function::Function, metric::MetricResults>> res;

    std::for_each(files.begin(), files.end(), [&](const std::string &filename) {
        const auto funcs = analyseFunctionsForFile(filename);

        std::for_each(funcs.cbegin(), funcs.cend(), [&metric_extractor, &res](const auto &func) {
            const auto metrics = metric_extractor.Get(func);
            res.emplace_back(func, metrics);
        });
    });

    return res;
}

void PrintAnalyseResults(const auto &v) {
    // TODO
}

auto SplitByClasses(const auto &analysis) {
    // здесь ваш код
}

auto SplitByFiles(const auto &analysis) {
    // здесь ваш код
}

void AccumulateFunctionAnalysis(const auto &analysis,
                                const analyser::metric_accumulator::MetricsAccumulator &accumulator) {
    // здесь ваш код
}

}  // namespace analyser
