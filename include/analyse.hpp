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
        const auto &[function, results] = p;
        std::string headline = function.file_name;

        if (function.class_name) {
            headline += "::"s + function.class_name.value();
        }

        headline += "::"s + function.function_name;
        std::print("\n{}\n", headline);

        std::ranges::for_each(results, [](const auto &m) { std::print("\t{}: {}\n", m.metric_name, m.metric_value); });
    });
}

auto SplitByClasses(auto &analysis) {
    const auto class_name_lambda = [](const auto &p) {
        return std::get<function::Function>(p).class_name.value_or("");
    };

    std::ranges::sort(analysis, [&class_name_lambda](const auto &lhd, const auto &rhd) {
        return class_name_lambda(lhd) < class_name_lambda(rhd);
    });

    auto chunks = analysis | std::views::chunk_by([&class_name_lambda](const auto &lhd, const auto &rhd) {
                      return class_name_lambda(lhd) == class_name_lambda(rhd);
                  }) |
                  std::views::drop(1);

    auto class_names =
        chunks | std::views::transform([&class_name_lambda](const auto &chunk) { return class_name_lambda(chunk[0]); });

    return std::views::zip(class_names, chunks);
}

auto SplitByFiles(auto &analysis) {
    const auto file_name_lambda = [](const auto &p) { return std::get<function::Function>(p).file_name; };

    std::ranges::sort(analysis, [&file_name_lambda](const auto &lhd, const auto &rhd) {
        return file_name_lambda(lhd) < file_name_lambda(rhd);
    });

    auto chunks = analysis | std::views::chunk_by([&file_name_lambda](const auto &lhd, const auto &rhd) {
                      return file_name_lambda(lhd) == file_name_lambda(rhd);
                  });

    auto file_names =
        chunks | std::views::transform([&file_name_lambda](const auto &chunk) { return file_name_lambda(chunk[0]); });

    return std::views::zip(file_names, chunks);
}

void AccumulateFunctionAnalysis(const auto &analysis, const metric_accumulator::MetricAccumulator &metric_accumulator) {
    std::ranges::for_each(analysis, [&metric_accumulator](const auto &p) {
        metric_accumulator.AccumulateNextResults(std::get<metric::MetricResults>(p));
    });
}

}  // namespace analyser
