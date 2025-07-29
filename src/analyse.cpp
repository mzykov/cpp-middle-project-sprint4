#include "analyse.hpp"

namespace analyser {

[[nodiscard]]
std::vector<std::pair<function::Function, metric::MetricResults>>
AnalyseFunctions(const std::vector<std::string> &file_names, const metric::MetricExtractor &metric_extractor) {
    const function::FunctionExtractor function_extractor;
    return file_names | std::views::transform([&](std::string_view file_name) {
               return function_extractor.ProcessOneFile(file::File{file_name});
           }) |
           std::views::join | std::views::transform([&](const function::Function &function) {
               const auto metrics = metric_extractor.ProcessOneFunction(function, function_extractor.GetAstExtractor());
               return std::pair{function, metrics};
           }) |
           std::ranges::to<std::vector<std::pair<function::Function, metric::MetricResults>>>();
}

}  // namespace analyser
