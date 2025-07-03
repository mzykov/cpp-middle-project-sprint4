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
    {
        using namespace analyser::metric;

        metric_extractor.RegisterMetric(
            std::make_unique<metric_impl::CodeLinesCountMetric>()
        );
        metric_extractor.RegisterMetric(
            std::make_unique<metric_impl::CyclomaticComplexityMetric>()
        );
        metric_extractor.RegisterMetric(
            std::make_unique<metric_impl::ParametersCountMetric>()
        );
    }

    auto analyseResults = analyser::AnalyseFunctions(
        options.GetFiles(), metric_extractor
    );

    analyser::PrintAnalyseResults(analyseResults);

    analyser::metric_accumulator::MetricsAccumulator accumulator;
    {
        using namespace analyser::metric_accumulator;

        accumulator.RegisterAccumulator(
            "AverageAccumulator", std::make_unique<metric_accumulator_impl::AverageAccumulator>()
        );
        accumulator.RegisterAccumulator(
            "CategoricalAccumulator", std::make_unique<metric_accumulator_impl::CategoricalAccumulator>()
        );
        accumulator.RegisterAccumulator(
            "SumAverageAccumulator", std::make_unique<metric_accumulator_impl::SumAverageAccumulator>()
        );
    }

    std::ranges::for_each(analyser::SplitByFiles(analyseResults),
        [&accumulator](const auto &p) {
            const auto &[file_name, chunk] = p;
            accumulator.ResetAccumulators();
            analyser::AccumulateFunctionAnalysis(chunk, accumulator);
            analyser::PrintAccumulatedAnalysisForFile(file_name, accumulator);
        }
    );

    std::ranges::for_each(analyser::SplitByClasses(analyseResults),
        [&accumulator](const auto &p) {
            const auto &[class_name, chunk] = p;
            accumulator.ResetAccumulators();
            analyser::AccumulateFunctionAnalysis(chunk, accumulator);
            analyser::PrintAccumulatedAnalysisForClass(class_name, accumulator);
        }
    );

    accumulator.ResetAccumulators();
    analyser::AccumulateFunctionAnalysis(analyseResults, accumulator);
    analyser::PrintAccumulatedAnalysisTotal(accumulator);

    return 0;
}
