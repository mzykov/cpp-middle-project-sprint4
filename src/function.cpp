#include "function.hpp"

#include <queue>

namespace analyser::function {

[[nodiscard]]
std::vector<Function> FunctionExtractor::ProcessOneFile(const file::File &file) const {
    std::vector<Function> result;
    std::queue<std::pair<std::string, size_t>> q;

    constexpr size_t start_parsing_at = 0;
    q.emplace(file.ast, start_parsing_at);

    while (!q.empty()) {
        const auto [ast_fragment, continue_parsing_at] = q.front();
        q.pop();

        const auto data = processASTFragment(file, ast_fragment, continue_parsing_at);

        if (data) {
            const auto &[func, further_parsing_at] = *data;
            result.push_back(func);
            q.emplace(ast_fragment, further_parsing_at);
            q.emplace(func.ast, ast_extractor_.FindPositionAfterFunctionDefinition(func.ast));
        }
    }

    return result;
}

std::optional<std::pair<Function, size_t>>
FunctionExtractor::processASTFragment(const file::File &file, std::string_view ast, size_t start_parsing_at) const {
    const auto data = ast_extractor_.ExtractFunctionDefinitionASTFragment(ast, start_parsing_at);

    if (!data) {
        return {};
    }

    const auto [func_ast, continue_parsing_at] = *data;
    const auto name_loc = ast_extractor_.GetNameLocation(func_ast);

    if (!name_loc) {
        return {};
    }

    const auto func_name = getNameFromSource(*name_loc, file.source_lines);

    std::optional<std::string> class_name;
    if (const auto class_loc = ast_extractor_.FindEnclosingClass(file.ast, *name_loc)) {
        class_name = getNameFromSource(*class_loc, file.source_lines);
    }

    bool is_decorated = false;
    const auto decorated_ast = ast_extractor_.FindEnclosingDecoratorAST(file.ast, *name_loc);

    if (decorated_ast) {
        const auto maybe_nested_name_loc = ast_extractor_.FirstFunctionDefinitionAfterDecorator(*decorated_ast);

        if (maybe_nested_name_loc && *maybe_nested_name_loc == *name_loc) {
            is_decorated = true;
        }
    }

    // clang-format off
    const auto func = Function{
        .file_name    = file.name,
        .class_name   = class_name,
        .func_name    = func_name,
        .ast          = is_decorated ? *decorated_ast : func_ast,
        .is_decorated = is_decorated,
    };
    // clang-format on

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
