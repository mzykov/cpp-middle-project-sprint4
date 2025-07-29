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

    metric_extractor.RegisterMetric(
        std::make_unique<analyser::metric::metric_impl::CodeLinesCountMetric>()
    );
    metric_extractor.RegisterMetric(
        std::make_unique<analyser::metric::metric_impl::CyclomaticComplexityMetric>()
    );
    metric_extractor.RegisterMetric(
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
        using namespace analyser::metric::metric_impl;

        metric_accumulator.RegisterAccumulator(
            ParametersCountMetric::ConstName(), std::make_shared<AverageAccumulator>()
        );
        metric_accumulator.RegisterAccumulator(
            CodeLinesCountMetric::ConstName(), std::make_shared<SumAverageAccumulator>()
        );
        metric_accumulator.RegisterAccumulator(
            CyclomaticComplexityMetric::ConstName(), std::make_shared<SumAverageAccumulator>()
        );
    }

    const auto accumulated_lambda = [&](const auto &chunk){
        using namespace analyser::metric_accumulator::metric_accumulator_impl;
        using namespace analyser::metric::metric_impl;

        metric_accumulator.ResetAccumulators();
        analyser::AccumulateFunctionAnalysis(chunk, metric_accumulator);

        const auto code_lines_acc = metric_accumulator.GetFinalizedAccumulator<SumAverageAccumulator>(
            CodeLinesCountMetric::ConstName()
        );
        std::println("\t{}: {}", CodeLinesCountMetric::ConstName(), code_lines_acc);

        const auto cyclomatic_acc = metric_accumulator.GetFinalizedAccumulator<SumAverageAccumulator>(
            CyclomaticComplexityMetric::ConstName()
        );
        std::println("\t{}: {}", CyclomaticComplexityMetric::ConstName(), cyclomatic_acc);

        const auto parameters_acc = metric_accumulator.GetFinalizedAccumulator<AverageAccumulator>(
            ParametersCountMetric::ConstName()
        );
        std::println("\t{}: {}", ParametersCountMetric::ConstName(), parameters_acc);
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
