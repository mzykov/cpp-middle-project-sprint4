#include "function.hpp"
#include "utils.hpp"

namespace analyser::function {

std::vector<Function> FunctionExtractor::Get(const file::File &file) const {
    std::vector<Function> functions;
    size_t start = 0;
    const std::string marker = "(function_definition";
    const std::string &ast = file.ast;

    while ((start = ast.find(marker, start)) != std::string::npos) {
        size_t open_braces = 1;
        size_t end = start + marker.length();

        while (end < ast.size() && open_braces > 0) {
            if (ast[end] == '(')
                open_braces++;
            else if (ast[end] == ')')
                open_braces--;
            end++;
        }

        auto func_ast = ast.substr(start, end - start);
        auto name_loc = getNameLocation(func_ast);
        std::string func_name = getNameFromSource(name_loc, file.source_lines);

        Function func{.filename = file.name, .class_name = std::nullopt, .name = func_name, .ast = func_ast};

        auto class_info = findEnclosingClass(ast, name_loc);
        if (class_info) {
            func.class_name = getClassNameFromSource(*class_info, file.source_lines);
        }

        functions.push_back(func);
        start = end;
    }

    return functions;
}

std::pair<Position, size_t> FunctionExtractor::extractPosition(const std::string &ast, size_t start_parsing_at) const {
    size_t coord_start = ast.find('[', start_parsing_at);
    size_t coord_end = ast.find(']', coord_start);

    std::string coords = ast.substr(coord_start + 1, coord_end - coord_start - 1);
    size_t comma = coords.find(',');

    return {Position{ToInt(coords.substr(0, comma)), ToInt(coords.substr(comma + 2))}, coord_end};
}

std::pair<Rect, size_t> FunctionExtractor::extractRect(const std::string &ast, size_t start_parsing_at) const {
    auto [start, next_parsing_at] = extractPosition(ast, start_parsing_at);
    auto [end, further_parsing_at] = extractPosition(ast, next_parsing_at);
    Rect rect{start, end};
    return {rect, further_parsing_at};
}

Rect FunctionExtractor::getNameLocation(const std::string &function_ast) const {
    size_t start_parsing_at = function_ast.find("(identifier");

    if (start_parsing_at == std::string::npos)
        return {};

    auto [rect, _] = extractRect(function_ast, start_parsing_at);

    return rect;
}

std::string FunctionExtractor::getNameFromSource(const Rect &func_rect, const std::vector<std::string> &lines) const {
    auto &start = std::get<0>(func_rect);
    auto &end = std::get<1>(func_rect);

    if (start.line >= lines.size())
        return "unknown";

    const std::string &target_line = lines[start.line];

    if (start.col >= target_line.size())
        return "unknown";

    return target_line.substr(start.col, end.col - start.col);
}

std::optional<Rect> FunctionExtractor::findEnclosingClass(const std::string &ast, const Rect &func_rect) const {
    size_t start_parsing_at = 0;
    const std::string class_marker = "(class_definition";
    std::optional<Rect> last_enclosing_class;

    while ((start_parsing_at = ast.find(class_marker, start_parsing_at)) != std::string::npos) {
        auto [rect, next_parsing_at] = extractRect(ast, start_parsing_at);
        const auto [class_start, class_end] = rect;
        const auto &start = std::get<0>(func_rect);

        if (start.line > class_start.line || (start.line == class_start.line && start.col >= class_start.col)) {
            if (start.line < class_end.line || (start.line == class_end.line && start.col <= class_end.col)) {
                size_t name_start = ast.find("name:", next_parsing_at);
                if (name_start != std::string::npos) {
                    size_t id_start = ast.find("(identifier", name_start);
                    if (id_start != std::string::npos) {
                        next_parsing_at = id_start;
                        last_enclosing_class = {class_start, class_end};
                    }
                }
            }
        }

        start_parsing_at = next_parsing_at;
    }

    return last_enclosing_class;
}

std::string FunctionExtractor::getClassNameFromSource(const Rect &class_rect,
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
