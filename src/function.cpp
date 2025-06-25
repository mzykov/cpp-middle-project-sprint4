#include "function.hpp"

namespace analyser::function {

std::vector<Function> FunctionExtractor::ProcessOneFile(const file::File &file) const {
    std::vector<Function> functions;
    size_t start = 0;
    const std::string marker = "(function_definition";
    const std::string &ast = file.ast;

    while (auto data = extractor_.extractASTFragment(ast, marker, start)) {
        auto [func_ast, end] = *data;
        auto name_loc = extractor_.getNameLocation(func_ast);

        if (name_loc) {
            std::string func_name = getFunctionNameFromSource(*name_loc, file.source_lines);
            std::optional<std::string> class_name;

            if (auto class_loc = extractor_.findEnclosingClass(ast, *name_loc)) {
                class_name = getClassNameFromSource(*class_loc, file.source_lines);
            }

            functions.emplace_back(file.name, class_name, func_name, func_ast);
        }

        start = end;
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

    const std::string &class_line = lines[start.line];

    size_t class_pos = class_line.find("class");
    if (class_pos == std::string::npos)
        return "unknown";

    size_t name_start = class_line.find_first_not_of(" \t", class_pos + 5);
    if (name_start == std::string::npos)
        return "unknown";

    size_t name_end = class_line.find_first_of(" :{(", name_start);
    if (name_end == std::string::npos)
        name_end = class_line.length();

    return class_line.substr(name_start, name_end - name_start);
}

}  // namespace analyser::function
