#include "analyse.hpp"

namespace analyser {

[[nodiscard]]
std::vector<std::pair<function::Function, metric::MetricResults>>
AnalyseFunctions(const std::vector<std::string> &file_names, const metric::MetricExtractor &metric_extractor) {
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

}  // namespace analyser
