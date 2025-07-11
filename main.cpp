#include "analyse.hpp"
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
        options.GetFiles(), metric_extractor
    );

    analyser::PrintAnalyseResults(analyseResults);

    analyser::metric_accumulator::MetricAccumulator metric_accumulator;
    {
        using namespace analyser::metric_accumulator;

        metric_accumulator.RegisterAccumulator(
            parameters_mptr->Name(), std::make_unique<metric_accumulator_impl::AverageAccumulator>()
        );
        metric_accumulator.RegisterAccumulator(
            code_lines_mptr->Name(), std::make_unique<metric_accumulator_impl::SumAverageAccumulator>()
        );
        metric_accumulator.RegisterAccumulator(
            cyclomatic_mptr->Name(), std::make_unique<metric_accumulator_impl::SumAverageAccumulator>()
        );
    }

    std::ranges::for_each(analyser::SplitByFiles(analyseResults),
        [&metric_accumulator](const auto &p) {
            const auto &[file_name, chunk] = p;
            metric_accumulator.ResetAccumulators();
            analyser::AccumulateFunctionAnalysis(chunk, metric_accumulator);
            analyser::PrintAccumulatedAnalysisForFile(file_name, metric_accumulator);
        }
    );

    std::ranges::for_each(analyser::SplitByClasses(analyseResults),
        [&metric_accumulator](const auto &p) {
            const auto &[class_name, chunk] = p;
            metric_accumulator.ResetAccumulators();
            analyser::AccumulateFunctionAnalysis(chunk, metric_accumulator);
            analyser::PrintAccumulatedAnalysisForClass(class_name, metric_accumulator);
        }
    );

    metric_accumulator.ResetAccumulators();
    analyser::AccumulateFunctionAnalysis(analyseResults, metric_accumulator);
    analyser::PrintAccumulatedAnalysisTotal(metric_accumulator);

    return 0;
}
