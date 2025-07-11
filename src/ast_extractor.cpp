#include <queue>

#include "ast_extractor.hpp"
#include "utils.hpp"

namespace analyser::ast_extractor {

std::optional<std::pair<std::string, size_t>>
ASTExtractor::ExtractFunctionDefinitionASTFragment(std::string_view ast, const size_t start_parsing_at) const {
    std::string_view marker = "(function_definition";
    return ExtractASTFragment(ast, marker, start_parsing_at);
}

std::optional<std::pair<std::string, size_t>>
ASTExtractor::ExtractIdentifierASTFragment(std::string_view ast, const size_t start_parsing_at) const {
    std::string_view marker = "(identifier";
    return ExtractASTFragment(ast, marker, start_parsing_at);
}

std::optional<std::pair<std::string, size_t>>
ASTExtractor::ExtractParametersASTFragment(std::string_view ast, const size_t start_parsing_at) const {
    std::string_view marker = "(parameters";
    return ExtractASTFragment(ast, marker, start_parsing_at);
}

std::optional<std::pair<std::string, size_t>>
ASTExtractor::ExtractCommentASTFragment(std::string_view ast, const size_t start_parsing_at) const {
    std::string_view marker = "(comment";
    return ExtractASTFragment(ast, marker, start_parsing_at);
}

std::unordered_set<size_t> ASTExtractor::ExtractAllCommentLineNumbers(std::string_view ast,
                                                                      const size_t start_parsing_at) const {
    std::unordered_set<size_t> res;
    std::queue<std::pair<std::string, size_t>> q;

    q.emplace(ast, start_parsing_at);

    while (!q.empty()) {
        const auto [ast_fragment, continue_parsing_at] = q.front();
        q.pop();

        const auto comment_data = ExtractCommentASTFragment(ast_fragment, continue_parsing_at);
        if (comment_data) {
            const auto [comment_ast, further_parsing_at] = *comment_data;
            q.emplace(ast_fragment, further_parsing_at);

            constexpr size_t start_parsing_comment_at = 0;
            const auto rect_data = ExtractRect(comment_ast, start_parsing_comment_at);

            if (rect_data) {
                const auto [rect, _] = *rect_data;
                const auto comment_line = ast::LinesInterval{rect};

                if (comment_line.IsOneLine()) {
                    res.insert(comment_line.start_line);
                }
            }
        }
    }

    return res;
}

size_t ASTExtractor::CountFirstLevelASTNodes(std::string_view ast) const { return CountNthLevelASTNodes(ast, 1); }

size_t ASTExtractor::CountNthLevelASTNodes(std::string_view ast, const size_t level) const {
    size_t res = 0, open_braces = 1, end = 1;

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
ASTExtractor::ExtractASTFragment(std::string_view ast, std::string_view marker_started_with_one_opened_brace,
                                 const size_t start_parsing_at) const {
    const auto marker_found_at = ast.find(marker_started_with_one_opened_brace, start_parsing_at);

    if (marker_found_at == std::string::npos) {
        return {};
    }

    size_t opened_braces = 1;
    size_t end = marker_found_at + marker_started_with_one_opened_brace.length();

    while (end < ast.length() && opened_braces > 0) {
        if (ast[end] == '(') {
            opened_braces++;
        } else if (ast[end] == ')') {
            opened_braces--;
        }
        end++;
    }

    return {{std::string{ast.substr(marker_found_at, end - marker_found_at)}, end}};
}

std::optional<std::pair<ast::Position, size_t>> ASTExtractor::extractPosition(std::string_view ast,
                                                                              const size_t start_parsing_at) const {
    const size_t coord_start = ast.find('[', start_parsing_at);
    if (coord_start == std::string::npos) {
        return {};
    }

    const size_t coord_end = ast.find(']', coord_start);
    if (coord_end == std::string::npos) {
        return {};
    }

    const auto coords = ast.substr(coord_start + 1, coord_end - coord_start - 1);
    const size_t comma = coords.find(',');
    if (comma == std::string::npos) {
        return {};
    }

    return {{ast::Position{ToInt(coords.substr(0, comma)), ToInt(coords.substr(comma + 2))}, coord_end}};
}

std::optional<std::pair<ast::Rect, size_t>> ASTExtractor::ExtractRect(std::string_view ast,
                                                                      const size_t start_parsing_at) const {
    const auto start_data = extractPosition(ast, start_parsing_at);
    if (!start_data) {
        return {};
    }
    const auto [start, continue_parsing_at] = *start_data;

    const auto end_data = extractPosition(ast, continue_parsing_at);
    if (!end_data) {
        return {};
    }
    const auto [end, further_parsing_at] = *end_data;

    return {{{start, end}, further_parsing_at}};
}

std::optional<ast::Rect> ASTExtractor::GetNameLocation(std::string_view ast) const {
    const auto data = ExtractIdentifierASTFragment(ast);

    if (!data)
        return {};

    constexpr size_t start_parsing_at = 0;
    const auto [id_ast, _] = *data;
    const auto rect_data = ExtractRect(id_ast, start_parsing_at);

    if (rect_data) {
        const auto [rect, _] = *rect_data;
        return {rect};
    } else {
        return {};
    }
}

std::optional<ast::Rect> ASTExtractor::FindEnclosingClass(std::string_view ast, const ast::Rect &func_rect,
                                                          const size_t start_parsing_at) const {
    std::string_view marker = "(class_definition";
    const auto ast_data = findEnclosingParentEntityAST(ast, marker, func_rect, start_parsing_at);

    if (ast_data) {
        return GetNameLocation(*ast_data);
    } else {
        return {};
    }
}

std::optional<std::string> ASTExtractor::FindEnclosingDecoratorAST(std::string_view ast, const ast::Rect &func_rect,
                                                                   const size_t start_parsing_at) const {
    std::string_view marker = "(decorated_definition";
    return findEnclosingParentEntityAST(ast, marker, func_rect, start_parsing_at);
}

std::optional<std::string> ASTExtractor::findEnclosingParentEntityAST(std::string_view ast, std::string_view marker,
                                                                      const ast::Rect &func_rect,
                                                                      const size_t start_parsing_at) const {
    std::optional<std::string> res;
    std::queue<std::pair<std::string, size_t>> q;

    const auto func_interval = ast::LinesInterval{func_rect};
    q.emplace(ast, start_parsing_at);

    while (!q.empty()) {
        const auto [ast_fragment, continue_parsing_at] = q.front();
        q.pop();

        const auto data = ExtractASTFragment(ast_fragment, marker, continue_parsing_at);
        if (data) {
            const auto [nested_ast, further_parsing_at] = *data;
            constexpr size_t start_find_rect_at = 0;
            const auto rect_data = ExtractRect(nested_ast, start_find_rect_at);

            if (rect_data) {
                const auto [parent_rect, _] = *rect_data;
                const auto parent_interval = ast::LinesInterval{parent_rect};

                if (parent_interval.Contains(func_interval)) {
                    res = nested_ast;
                    constexpr std::string_view opened_brace = "(";
                    q.emplace(nested_ast, opened_brace.length());
                    continue;
                }
            }

            q.emplace(ast_fragment, further_parsing_at);
        }
    }

    return res;
}

size_t ASTExtractor::FindPositionAfterFunctionDefinition(std::string_view ast, const size_t start_parsing_at) const {
    constexpr std::string_view marker = "(function_definition";  // TODO: remove this duplicate definition
    const auto pos = ast.find(marker, start_parsing_at);

    if (pos == std::string::npos) {
        return pos;
    } else {
        return pos + marker.length();
    }
}

std::optional<ast::Rect> ASTExtractor::FirstFunctionDefinitionAfterDecorator(std::string_view ast) const {
    const auto data = ExtractFunctionDefinitionASTFragment(ast);

    if (data) {
        const auto [func_ast, _] = *data;
        return GetNameLocation(func_ast);
    } else {
        return {};
    }
}

}  // namespace analyser::ast_extractor
