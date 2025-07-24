#include "analyse.hpp"
#include "analyse_split.hpp"
#include "cmd_options.hpp"
#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"
#include "metric_accumulator_impl/accumulators.hpp"
#include "metric_impl/metrics.hpp"

#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[]) {
    analyser::cmd::ProgramOptions options;
    options.Parse(argc, argv);

    analyser::metric::MetricExtractor metric_extractor;

    auto code_lines_metric_name = metric_extractor.RegisterMetric(
        std::make_unique<analyser::metric::metric_impl::CodeLinesCountMetric>()
    );
    auto cyclomatic_metric_name = metric_extractor.RegisterMetric(
        std::make_unique<analyser::metric::metric_impl::CyclomaticComplexityMetric>()
    );
    auto parameters_metric_name = metric_extractor.RegisterMetric(
        std::make_unique<analyser::metric::metric_impl::ParametersCountMetric>()
    );

    std::vector<std::pair<analyser::function::Function, analyser::metric::MetricResults>> analyseResults;

    try {
        analyseResults = analyser::AnalyseFunctions(
            options.GetFileNames(), metric_extractor
        );
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    analyser::PrintAnalyseResults(analyseResults);

    analyser::metric_accumulator::MetricAccumulator metric_accumulator;
    {
        using namespace analyser::metric_accumulator::metric_accumulator_impl;

        metric_accumulator.RegisterAccumulator(
            parameters_metric_name, std::make_shared<AverageAccumulator>()
        );
        metric_accumulator.RegisterAccumulator(
            code_lines_metric_name, std::make_shared<SumAverageAccumulator>()
        );
        metric_accumulator.RegisterAccumulator(
            cyclomatic_metric_name, std::make_shared<SumAverageAccumulator>()
        );
    }

    const auto accumulated_lambda = [&](const auto &chunk){
        using namespace analyser::metric_accumulator::metric_accumulator_impl;

        metric_accumulator.ResetAccumulators();
        analyser::AccumulateFunctionAnalysis(chunk, metric_accumulator);

        const auto code_lines_acc = metric_accumulator.GetFinalizedAccumulator<SumAverageAccumulator>(
            code_lines_metric_name
        );
        std::println("\t{}: {}", code_lines_metric_name, code_lines_acc);

        const auto cyclomatic_acc = metric_accumulator.GetFinalizedAccumulator<SumAverageAccumulator>(
            cyclomatic_metric_name
        );
        std::println("\t{}: {}", cyclomatic_metric_name, cyclomatic_acc);

        const auto parameters_acc = metric_accumulator.GetFinalizedAccumulator<AverageAccumulator>(
            parameters_metric_name
        );
        std::println("\t{}: {}", parameters_metric_name, parameters_acc);
    };

    {
        std::ranges::for_each(
            analyser::SplitByFiles(analyseResults),
            [&](const auto &p) {
                const auto &[file_name, chunk] = p;
                std::println("\nAccumulated Analysis for file {}", file_name);
                accumulated_lambda(chunk);
            }
        );
    }
    {
        std::ranges::for_each(
            analyser::SplitByClasses(analyseResults),
            [&](const auto &p) {
                const auto &[class_name, chunk] = p;
                std::println("\nAccumulated Analysis for class {}", class_name);
                accumulated_lambda(chunk);
            }
        );
    }
    {
        std::println("\nAccumulated Analysis total");
        accumulated_lambda(analyseResults);
    }

    return EXIT_SUCCESS;
}
