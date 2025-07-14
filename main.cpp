#include "analyse.hpp"
#include "analyse_split.hpp"
#include "cmd_options.hpp"
#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"
#include "metric_accumulator_impl/accumulators.hpp"
#include "metric_impl/metrics.hpp"

int main(int argc, char *argv[]) {
    analyser::cmd::ProgramOptions options;
    options.Parse(argc, argv);

    analyser::metric::MetricExtractor metric_extractor;

    auto code_lines_mptr = std::make_unique<analyser::metric::metric_impl::CodeLinesCountMetric>();
    metric_extractor.RegisterMetric(std::move(code_lines_mptr));

    auto cyclomatic_mptr = std::make_unique<analyser::metric::metric_impl::CyclomaticComplexityMetric>();
    metric_extractor.RegisterMetric(std::move(cyclomatic_mptr));

    auto parameters_mptr = std::make_unique<analyser::metric::metric_impl::ParametersCountMetric>();
    metric_extractor.RegisterMetric(std::move(parameters_mptr));

    auto analyseResults = analyser::AnalyseFunctions(
        options.GetFileNames(), metric_extractor
    );

    analyser::PrintAnalyseResults(analyseResults);

    analyser::metric_accumulator::MetricAccumulator metric_accumulator;
    {
        using namespace analyser::metric_accumulator::metric_accumulator_impl;

        metric_accumulator.RegisterAccumulator(
            parameters_mptr->Name(), std::make_shared<AverageAccumulator>()
        );
        metric_accumulator.RegisterAccumulator(
            code_lines_mptr->Name(), std::make_shared<SumAverageAccumulator>()
        );
        metric_accumulator.RegisterAccumulator(
            cyclomatic_mptr->Name(), std::make_shared<SumAverageAccumulator>()
        );
    }

    const auto accumulated_lambda = [&](const auto &chunk){
        using namespace analyser::metric_accumulator::metric_accumulator_impl;

        metric_accumulator.ResetAccumulators();
        analyser::AccumulateFunctionAnalysis(chunk, metric_accumulator);

        const auto code_lines_acc = metric_accumulator.GetFinalizedAccumulator<SumAverageAccumulator>(
            code_lines_mptr->Name()
        );
        std::println("\t{}: {}", code_lines_mptr->Name(), code_lines_acc);

        const auto cyclomatic_acc = metric_accumulator.GetFinalizedAccumulator<SumAverageAccumulator>(
            cyclomatic_mptr->Name()
        );
        std::println("\t{}: {}", cyclomatic_mptr->Name(), cyclomatic_acc);

        const auto parameters_acc = metric_accumulator.GetFinalizedAccumulator<AverageAccumulator>(
            parameters_mptr->Name()
        );
        std::println("\t{}: {}", parameters_mptr->Name(), parameters_acc);
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

    return 0;
}
