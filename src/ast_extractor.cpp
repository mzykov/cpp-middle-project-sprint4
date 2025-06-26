#include "ast_extractor.hpp"
#include "utils.hpp"

namespace analyser::ast_extractor {

std::optional<std::pair<std::string, size_t>>
ASTExtractor::ExtractClassDefenitionASTFragment(const std::string &ast, size_t start_parsing_at) const {
    const std::string marker = "(class_defenition";
    return extractASTFragment(ast, marker, start_parsing_at);
}

std::optional<std::pair<std::string, size_t>>
ASTExtractor::ExtractFunctionDefenitionASTFragment(const std::string &ast, size_t start_parsing_at) const {
    const std::string marker = "(function_defenition";
    return extractASTFragment(ast, marker, start_parsing_at);
}

std::optional<std::pair<std::string, size_t>>
ASTExtractor::ExtractIdentifierASTFragment(const std::string &ast, size_t start_parsing_at) const {
    const std::string marker = "(identifier";
    return extractASTFragment(ast, marker, start_parsing_at);
}

std::optional<std::pair<std::string, size_t>>
ASTExtractor::ExtractParametersASTFragment(const std::string &ast, size_t start_parsing_at) const {
    const std::string marker = "(parameters";
    return extractASTFragment(ast, marker, start_parsing_at);
}

size_t ASTExtractor::CountFirstLevelASTNodes(const std::string &ast) const { return CountNthLevelASTNodes(ast, 1); }

size_t ASTExtractor::CountNthLevelASTNodes(const std::string &ast_begins_with_one_opened_brace, size_t level) const {
    size_t open_braces = 1, end = 1;
    size_t res = 0;
    const auto &ast = ast_begins_with_one_opened_brace;

    while (end < ast.size() && open_braces > 0) {
        if (ast[end] == '(') {
            open_braces++;
        } else if (ast[end] == ')') {
            open_braces--;
            if (open_braces == level) {
                ++res;
            }
        }
        end++;
    }

    return res;
}

std::optional<std::pair<std::string, size_t>>
ASTExtractor::extractASTFragment(const std::string &ast, const std::string &marker_with_one_opened_brace,
                                 size_t start_parsing_at) const {

    auto marker_found_at = ast.find(marker_with_one_opened_brace, start_parsing_at);

    if (marker_found_at == std::string::npos) {
        return {};
    }

    size_t open_braces = 1;
    size_t end = marker_found_at + marker_with_one_opened_brace.length();

    while (end < ast.size() && open_braces > 0) {
        if (ast[end] == '(') {
            open_braces++;
        } else if (ast[end] == ')') {
            open_braces--;
        }
        end++;
    }

    std::optional<std::pair<std::string, size_t>> res;
    res = {ast.substr(marker_found_at, end - marker_found_at), end};

    return res;
}

std::pair<ast::Position, size_t> ASTExtractor::extractPosition(const std::string &ast, size_t start_parsing_at) const {
    size_t coord_start = ast.find('[', start_parsing_at);
    size_t coord_end = ast.find(']', coord_start);

    std::string coords = ast.substr(coord_start + 1, coord_end - coord_start - 1);
    size_t comma = coords.find(',');

    return {ast::Position{ToInt(coords.substr(0, comma)), ToInt(coords.substr(comma + 2))}, coord_end};
}

std::pair<ast::Rect, size_t> ASTExtractor::extractRect(const std::string &ast, size_t start_parsing_at) const {
    auto [start, next_parsing_at] = extractPosition(ast, start_parsing_at);
    auto [end, further_parsing_at] = extractPosition(ast, next_parsing_at);
    ast::Rect rect{start, end};
    return {rect, further_parsing_at};
}

std::optional<ast::Rect> ASTExtractor::getNameLocation(const std::string &func_ast) const {
    auto data = ExtractIdentifierASTFragment(func_ast);

    if (!data)
        return {};

    auto [id_ast, start_parsing_at] = *data;
    auto [rect, _] = extractRect(id_ast, 0);

    std::optional<ast::Rect> res;
    res = rect;

    return res;
}

std::optional<ast::Rect> ASTExtractor::findEnclosingClass(const std::string &ast, const ast::Rect &func_rect) const {
    size_t start_parsing_at = 0;
    const std::string class_marker = "(class_definition";
    std::optional<ast::Rect> last_enclosing_class;

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

}  // namespace analyser::ast_extractor
