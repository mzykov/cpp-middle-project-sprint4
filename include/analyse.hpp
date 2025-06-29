#include <print>

#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"

namespace analyser {

auto AnalyseFunctions(const std::vector<std::string> &files, const metric::MetricExtractor &metric_extractor) {
    std::vector<std::pair<function::Function, metric::MetricResults>> res;

    std::for_each(files.cbegin(), files.cend(), [&](const std::string &filename) {
        const function::FunctionExtractor extractor;
        const auto funcs = extractor.ProcessOneFile(file::File{filename});

        std::for_each(funcs.cbegin(), funcs.cend(), [&metric_extractor, &res](const auto &func) {
            const auto metrics = metric_extractor.ProcessOneFunction(func);
            res.emplace_back(func, metrics);
        });
    });

    return res;
}

void PrintAnalyseResults(const auto &analysis) {
    using namespace std::string_literals;

    for (const auto &p : analysis) {
        const auto &[func, results] = p;
        std::string headline = func.filename;
        if (func.class_name) {
            headline += "::"s + *func.class_name;
        }
        headline += "::"s + func.name;
        std::print("\n{}\n", headline);

        std::for_each(results.cbegin(), results.cend(),
                      [](const auto &m) { std::print("\t{}: {}\n", m.metric_name, m.metric_value); });
    }
}

auto SplitByClasses(const auto &analysis) {
    // здесь ваш код
}

void PrintAccumulatedAnalysisForClass(const auto &analysis) {
    for (const auto &p : analysis) {
        const auto &[class_name, results] = p;
        std::print("\nAccumulated Analysis for сlass {}\n", class_name);
        std::for_each(results.cbegin(), results.cend(),
                      [](const auto &m) { std::print("\t{}: {}\n", m.metric_name, m.metric_value); });
    }
}

auto SplitByFiles(const auto &analysis) {
    // здесь ваш код
}

void PrintAccumulatedAnalysisForFile(const auto &analysis) {
    for (const auto &p : analysis) {
        const auto &[file_name, results] = p;
        std::print("\nAccumulated Analysis for file {}\n", file_name);
        std::for_each(results.cbegin(), results.cend(),
                      [](const auto &m) { std::print("\t{}: {}\n", m.metric_name, m.metric_value); });
    }
}

void AccumulateFunctionAnalysis(const auto &analysis, const metric_accumulator::MetricsAccumulator &accumulator) {
    // здесь ваш код
}

}  // namespace analyser
