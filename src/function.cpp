#include "function.hpp"

namespace analyser::function {

std::vector<Function> FunctionExtractor::ProcessOneFile(const file::File &file) const {
    std::vector<Function> functions;
    size_t start_parsing_at = 0;
    const std::string &ast = file.ast;

    while (auto data = extractor_.ExtractFunctionDefinitionASTFragment(ast, start_parsing_at)) {
        auto [func_ast, continue_parsing_at] = *data;
        auto name_loc = extractor_.getNameLocation(func_ast);

        if (name_loc) {
            std::string func_name = getNameFromSource(*name_loc, file.source_lines);
            std::optional<std::string> class_name;

            if (auto class_loc = extractor_.findEnclosingClass(ast, *name_loc)) {
                class_name = getNameFromSource(*class_loc, file.source_lines);
            }

            functions.emplace_back(file.name, class_name, func_name, func_ast);
        }

        start_parsing_at = continue_parsing_at;
    }

    return functions;
}

std::string FunctionExtractor::getNameFromSource(const ast::Rect &rect, const std::vector<std::string> &lines) const {
    auto &start = std::get<0>(rect);
    auto &end = std::get<1>(rect);

    if (start.line >= lines.size())
        return "unknown";

    const std::string &target_line = lines[start.line];

    if (start.col >= target_line.size())
        return "unknown";

    return target_line.substr(start.col, end.col - start.col);
}

}  // namespace analyser::function
