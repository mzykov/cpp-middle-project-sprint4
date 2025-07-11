#include <algorithm>
#include <print>
#include <ranges>

#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"

namespace analyser {

[[nodiscard]]
auto AnalyseFunctions(const std::vector<std::string> &file_names, const metric::MetricExtractor &metric_extractor) {
    std::vector<std::pair<function::Function, metric::MetricResults>> res;

    std::ranges::for_each(file_names, [&metric_extractor, &res](std::string_view file_name) {
        const function::FunctionExtractor function_extractor;
        const auto functions = function_extractor.ProcessOneFile(file::File{file_name});

        std::ranges::for_each(functions, [&function_extractor, &metric_extractor, &res](const auto &function) {
            const auto metrics = metric_extractor.ProcessOneFunction(function, function_extractor.GetAstExtractor());
            res.emplace_back(function, metrics);
        });
    });

    return res;
}

void PrintAnalyseResults(const auto &analysis) {
    using namespace std::string_literals;

    std::ranges::for_each(analysis, [](const auto &p) {
        const auto &[func, results] = p;
        std::string headline = func.file_name;

        if (func.class_name) {
            headline += "::"s + *func.class_name;
        }

        headline += "::"s + func.func_name;
        std::print("\n{}\n", headline);

        std::ranges::for_each(results, [](const auto &m) { std::print("\t{}: {}\n", m.metric_name, m.metric_value); });
    });
}

auto SplitByClasses(auto &analysis) {
    std::ranges::sort(analysis, [](const auto &lhd, const auto &rhd) {
        const std::string empty = "";
        const auto l = std::get<function::Function>(lhd).class_name.value_or(empty);
        const auto r = std::get<function::Function>(rhd).class_name.value_or(empty);
        return l < r;
    });

    auto chunks = analysis | std::views::chunk_by([](const auto &lhd, const auto &rhd) {
                      const std::string empty = "";
                      const auto l = std::get<function::Function>(lhd).class_name.value_or(empty);
                      const auto r = std::get<function::Function>(rhd).class_name.value_or(empty);
                      return l == r;
                  }) |
                  std::views::drop(1);

    auto class_names = chunks | std::views::transform([](const auto &chunk) {
                           return std::get<function::Function>(chunk[0]).class_name.value();
                       });

    return std::views::zip(class_names, chunks);
}

void PrintAccumulatedAnalysisForClass(const std::string &class_name,
                                      const metric_accumulator::MetricAccumulator &accumulator) {
    std::print("\nAccumulated Analysis for сlass {}\n", class_name);
}

auto SplitByFiles(auto &analysis) {
    std::ranges::sort(analysis, [](const auto &lhd, const auto &rhd) {
        return std::get<function::Function>(lhd).file_name < std::get<function::Function>(rhd).file_name;
    });
    auto chunks = analysis | std::views::chunk_by([](const auto &lhd, const auto &rhd) {
                      return std::get<function::Function>(lhd).file_name == std::get<function::Function>(rhd).file_name;
                  });
    auto file_names = chunks | std::views::transform(
                                   [](const auto &chunk) { return std::get<function::Function>(chunk[0]).file_name; });
    return std::views::zip(file_names, chunks);
}

void PrintAccumulatedAnalysisForFile(const std::string &file_name,
                                     const metric_accumulator::MetricAccumulator &accumulator) {
    std::print("\nAccumulated Analysis for file {}\n", file_name);
    /*
        for (const auto &[metric_name, ] : accumulator.GetAccumulators()) {
            std::for_each(results.cbegin(), results.cend(), [](const auto &m) {
                std::print("\t{}: {}\n", m.metric_name, m.metric_value);
            });
        }
    */
}

void AccumulateFunctionAnalysis(const auto &analysis, const metric_accumulator::MetricAccumulator &accumulator) {
    // здесь ваш код
}

void PrintAccumulatedAnalysisTotal(const metric_accumulator::MetricAccumulator &accumulator) {
    std::print("\nAccumulated Analysis total\n");
}

}  // namespace analyser
