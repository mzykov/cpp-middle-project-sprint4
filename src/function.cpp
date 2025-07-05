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
            std::string func_name = getFunctionNameFromSource(*name_loc, file.source_lines);
            std::optional<std::string> class_name;

            if (auto class_loc = extractor_.findEnclosingClass(ast, *name_loc)) {
                class_name = getClassNameFromSource(*class_loc, file.source_lines);
            }

            functions.emplace_back(file.name, class_name, func_name, func_ast);
        }

        start_parsing_at = continue_parsing_at;
    }

    return functions;
}

std::string FunctionExtractor::getFunctionNameFromSource(const ast::Rect &func_rect,
                                                         const std::vector<std::string> &lines) const {
    auto &start = std::get<0>(func_rect);
    auto &end = std::get<1>(func_rect);

    if (start.line >= lines.size())
        return "unknown";

    const std::string &target_line = lines[start.line];

    if (start.col >= target_line.size())
        return "unknown";

    return target_line.substr(start.col, end.col - start.col);
}

std::string FunctionExtractor::getClassNameFromSource(const ast::Rect &class_rect,
                                                      const std::vector<std::string> &lines) const {
    auto start = std::get<0>(class_rect);

    if (start.line >= lines.size())
        return "unknown";

    std::string_view class_line{lines[start.line]};
    std::string_view class_kw = "class";

    size_t class_pos = class_line.find(class_kw);
    if (class_pos == std::string::npos)
        return "unknown";

    size_t name_start = class_line.find_first_not_of(" \t", class_pos + class_kw.length());
    if (name_start == std::string::npos)
        return "unknown";

    size_t name_end = class_line.find_first_of(" :{(", name_start);
    if (name_end == std::string::npos)
        name_end = class_line.length();

    return std::string{class_line.substr(name_start, name_end - name_start)};
}

}  // namespace analyser::function
