#include <queue>

#include "function.hpp"

namespace analyser::function {

[[nodiscard]]
std::vector<Function> FunctionExtractor::ProcessOneFile(const file::File &file) const {
    std::vector<Function> result;
    std::queue<std::pair<std::string, size_t>> q;

    const size_t start_parsing_at = 0;
    q.emplace(file.ast, start_parsing_at);

    while (!q.empty()) {
        const auto [ast_fragment, continue_parsing_at] = q.front();
        q.pop();

        const auto data = processASTFragment(file, ast_fragment, continue_parsing_at);

        if (data) {
            constexpr std::string_view opened_brace = "(";
            const auto &[func, further_parsing_at] = *data;
            result.push_back(func);
            q.emplace(ast_fragment, further_parsing_at);
            q.emplace(func.ast, start_parsing_at + opened_brace.length());
        }
    }

    return result;
}

std::optional<std::pair<Function, size_t>>
FunctionExtractor::processASTFragment(const file::File &file, std::string_view ast, size_t start_parsing_at) const {
    const auto data = extractor_.ExtractFunctionDefinitionASTFragment(ast, start_parsing_at);

    if (!data) {
        return {};
    }

    const auto [func_ast, continue_parsing_at] = *data;
    const auto name_loc = extractor_.getNameLocation(func_ast);

    if (!name_loc) {
        return {};
    }

    const auto func_name = getNameFromSource(*name_loc, file.source_lines);
    std::optional<std::string> class_name;

    if (const auto class_loc = extractor_.findEnclosingClass(file.ast, *name_loc)) {
        class_name = getNameFromSource(*class_loc, file.source_lines);
    }

    const auto func =
        Function{.file_name = file.name, .class_name = class_name, .func_name = func_name, .ast = func_ast};

    return {{func, continue_parsing_at}};
}

std::string FunctionExtractor::getNameFromSource(const ast::Rect &rect, const std::vector<std::string> &lines) const {
    const auto start = std::get<0>(rect);
    const auto end = std::get<1>(rect);

    if (start.line >= lines.size())
        return "unknown";

    std::string_view target_line = lines[start.line];

    if (start.col >= target_line.size())
        return "unknown";

    return std::string{target_line.substr(start.col, end.col - start.col)};
}

}  // namespace analyser::function
