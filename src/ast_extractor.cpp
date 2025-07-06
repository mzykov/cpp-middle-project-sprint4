#include "ast_extractor.hpp"
#include "utils.hpp"

namespace analyser::ast_extractor {

std::optional<std::pair<std::string, size_t>>
ASTExtractor::ExtractClassDefinitionASTFragment(std::string_view ast, size_t start_parsing_at) const {
    std::string_view marker = "(class_definition";
    return extractASTFragment(ast, marker, start_parsing_at);
}

std::optional<std::pair<std::string, size_t>>
ASTExtractor::ExtractFunctionDefinitionASTFragment(std::string_view ast, size_t start_parsing_at) const {
    std::string_view marker = "(function_definition";
    return extractASTFragment(ast, marker, start_parsing_at);
}

std::optional<std::pair<std::string, size_t>>
ASTExtractor::ExtractDecoratedDefinitionASTFragment(std::string_view ast, size_t start_parsing_at) const {
    std::string_view marker = "(decorated_definition";
    return extractASTFragment(ast, marker, start_parsing_at);
}

std::optional<std::pair<std::string, size_t>> ASTExtractor::ExtractDecoratorASTFragment(std::string_view ast,
                                                                                        size_t start_parsing_at) const {
    std::string_view marker = "(decorator";
    return extractASTFragment(ast, marker, start_parsing_at);
}

std::optional<std::pair<std::string, size_t>>
ASTExtractor::ExtractIdentifierASTFragment(std::string_view ast, size_t start_parsing_at) const {
    std::string_view marker = "(identifier";
    return extractASTFragment(ast, marker, start_parsing_at);
}

std::optional<std::pair<std::string, size_t>>
ASTExtractor::ExtractParametersASTFragment(std::string_view ast, size_t start_parsing_at) const {
    std::string_view marker = "(parameters";
    return extractASTFragment(ast, marker, start_parsing_at);
}

std::optional<std::pair<std::string, size_t>> ASTExtractor::ExtractCommentASTFragment(std::string_view ast,
                                                                                      size_t start_parsing_at) const {
    std::string_view marker = "(comment";
    return extractASTFragment(ast, marker, start_parsing_at);
}

std::unordered_set<size_t> ASTExtractor::extractAllCommentLineNumbers(std::string_view ast,
                                                                      size_t start_parsing_at) const {
    std::unordered_set<size_t> res;

    while (auto comment_data = ExtractCommentASTFragment(ast, start_parsing_at)) {
        auto [comment_ast, continue_parsing_at] = *comment_data;
        auto rect_data = extractRect(comment_ast, 0);

        if (!rect_data) {
            break;
        }

        auto [rect, _] = *rect_data;
        auto comment_line = ast::LinesInterval{rect};

        if (comment_line.IsOneLine()) {
            res.insert(comment_line.start_line);
        }

        start_parsing_at = continue_parsing_at;
    }

    return res;
}

size_t ASTExtractor::CountFirstLevelASTNodes(std::string_view ast) const { return CountNthLevelASTNodes(ast, 1); }

size_t ASTExtractor::CountNthLevelASTNodes(std::string_view ast, size_t level) const {
    size_t open_braces = 1, end = 1;
    size_t res = 0;

    while (end < ast.length() && open_braces > 0) {
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
ASTExtractor::extractASTFragment(std::string_view ast, std::string_view marker_started_with_one_opened_brace,
                                 size_t start_parsing_at) const {
    auto marker_found_at = ast.find(marker_started_with_one_opened_brace, start_parsing_at);

    if (marker_found_at == std::string::npos) {
        return {};
    }

    size_t open_braces = 1;
    size_t end = marker_found_at + marker_started_with_one_opened_brace.length();

    while (end < ast.length() && open_braces > 0) {
        if (ast[end] == '(') {
            open_braces++;
        } else if (ast[end] == ')') {
            open_braces--;
        }
        end++;
    }

    return {{std::string{ast.substr(marker_found_at, end - marker_found_at)}, end}};
}

std::optional<std::pair<ast::Position, size_t>> ASTExtractor::extractPosition(std::string_view ast,
                                                                              size_t start_parsing_at) const {
    size_t coord_start = ast.find('[', start_parsing_at);
    if (coord_start == std::string::npos) {
        return {};
    }

    size_t coord_end = ast.find(']', coord_start);
    if (coord_end == std::string::npos) {
        return {};
    }

    auto coords = ast.substr(coord_start + 1, coord_end - coord_start - 1);
    size_t comma = coords.find(',');
    if (comma == std::string::npos) {
        return {};
    }

    return {{ast::Position{ToInt(coords.substr(0, comma)), ToInt(coords.substr(comma + 2))}, coord_end}};
}

std::optional<std::pair<ast::Rect, size_t>> ASTExtractor::extractRect(std::string_view ast,
                                                                      size_t start_parsing_at) const {
    auto start_data = extractPosition(ast, start_parsing_at);
    if (!start_data) {
        return {};
    }
    auto [start, continue_parsing_at] = *start_data;

    auto end_data = extractPosition(ast, continue_parsing_at);
    if (!end_data) {
        return {};
    }
    auto [end, further_parsing_at] = *end_data;

    return {{{start, end}, further_parsing_at}};
}

std::optional<ast::Rect> ASTExtractor::getNameLocation(std::string_view ast) const {
    auto data = ExtractIdentifierASTFragment(ast);

    if (!data)
        return {};

    auto [id_ast, start_parsing_at] = *data;
    auto rect_data = extractRect(id_ast, 0);

    if (rect_data) {
        auto [rect, _] = *rect_data;
        return {rect};
    } else {
        return {};
    }
}

std::optional<ast::Rect> ASTExtractor::findEnclosingClass(std::string_view ast, const ast::Rect &func_rect,
                                                          size_t start_parsing_at) const {
    std::optional<ast::Rect> last_enclosing_class;
    auto func_interval = ast::LinesInterval{func_rect};

    while (auto class_data = ExtractClassDefinitionASTFragment(ast, start_parsing_at)) {
        auto [class_ast, continue_parsing_at] = *class_data;
        auto rect_data = extractRect(class_ast, 0);

        if (!rect_data) {
            break;
        }

        auto [class_rect, _] = *rect_data;
        auto class_interval = ast::LinesInterval{class_rect};

        if (class_interval.Contains(func_interval)) {
            if (auto class_loc = findEnclosingClass(class_ast, func_rect, 1)) {
                return class_loc;
            } else {
                return getNameLocation(class_ast);
            }
        }

        start_parsing_at = continue_parsing_at;
    }

    return last_enclosing_class;
}

}  // namespace analyser::ast_extractor
