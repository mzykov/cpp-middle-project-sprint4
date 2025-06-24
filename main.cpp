#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

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

    // analyser::metric_accumulator::MetricsAccumulator accumulator;
    // зарегистрируйте аккумуляторы метрик в accumulator

    // запустите analyser::SplitByFiles
    // запустите analyser::AccumulateFunctionAnalysis для каждого подмножества результатов метрик
    // выведете результаты на консоль

    // запустите analyser::SplitByClasses
    // запустите analyser::AccumulateFunctionAnalysis для каждого подмножества результатов метрик
    // выведете результаты на консоль

    // запустите analyser::AccumulateFunctionAnalysis для всех результатов метрик
    // выведете результаты на консоль

    return 0;
}
