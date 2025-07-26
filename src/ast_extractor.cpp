#include <queue>

#include "ast_extractor.hpp"
#include "utils.hpp"

namespace analyser::ast_extractor {

std::optional<std::pair<std::string_view, size_t>>
ASTExtractor::ExtractFunctionDefinitionASTFragment(std::string_view ast, const size_t start_parsing_at) const {
    return ExtractASTFragment(ast, getDefinitionKeyword("function"), start_parsing_at);
}

std::optional<std::pair<std::string_view, size_t>>
ASTExtractor::ExtractIdentifierASTFragment(std::string_view ast, const size_t start_parsing_at) const {
    return ExtractASTFragment(ast, getDefinitionKeyword("identifier"), start_parsing_at);
}

std::optional<std::pair<std::string_view, size_t>>
ASTExtractor::ExtractParametersASTFragment(std::string_view ast, const size_t start_parsing_at) const {
    return ExtractASTFragment(ast, getDefinitionKeyword("parameters"), start_parsing_at);
}

std::optional<std::pair<std::string_view, size_t>>
ASTExtractor::ExtractCommentASTFragment(std::string_view ast, const size_t start_parsing_at) const {
    return ExtractASTFragment(ast, getDefinitionKeyword("comment"), start_parsing_at);
}

std::unordered_set<size_t> ASTExtractor::ExtractAllCommentLineNumbers(std::string_view ast,
                                                                      const size_t start_parsing_at) const {
    std::unordered_set<size_t> res;
    std::queue<std::pair<std::string_view, size_t>> q;

    q.emplace(ast, start_parsing_at);

    // clang-format off
    while (!q.empty()) {
        const auto [ast_fragment, continue_parsing_at] = q.front();
        q.pop();
        ExtractCommentASTFragment(ast_fragment, continue_parsing_at)
            .and_then([&](const auto &ast_data) {
                const auto [comment_ast, further_parsing_at] = ast_data;
                q.emplace(ast_fragment, further_parsing_at);
                constexpr size_t start_parsing_comment_at = 0;
                return ExtractRect(comment_ast, start_parsing_comment_at);
            })
            .transform([](const auto &rect_data) {
                return ast::LinesInterval{std::get<ast::Rect>(rect_data)};
            })
            .and_then([](const auto &comment_line) {
                if (comment_line.IsOneLine()) {
                    return std::optional<size_t>{comment_line.start_line};
                } else {
                    return std::optional<size_t>{};
                }
            })
            .transform([&res](const auto comment_line_number) {
                return res.insert(comment_line_number);
            });
    }
    // clang-format on

    return res;
}

size_t ASTExtractor::CountFirstLevelASTNodes(std::string_view ast) const {
    constexpr size_t first_level = 1;
    return CountNthLevelASTNodes(ast, first_level);
}

size_t ASTExtractor::CountNthLevelASTNodes(std::string_view ast, const size_t level) const {
    size_t res = 0, opened_braces = 1, end = 1;

    while (end < ast.length() && opened_braces > 0) {
        if (ast[end] == '(') {
            opened_braces++;
        } else if (ast[end] == ')') {
            opened_braces--;
            if (opened_braces == level) {
                ++res;
            }
        }
        end++;
    }

    return res;
}

std::optional<std::pair<std::string_view, size_t>>
ASTExtractor::ExtractASTFragment(std::string_view ast, std::string_view marker_started_with_one_opened_brace,
                                 const size_t start_parsing_at) const {
    const auto marker_found_at = ast.find(marker_started_with_one_opened_brace, start_parsing_at);

    if (marker_found_at == std::string_view::npos) {
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

    return {{ast.substr(marker_found_at, end - marker_found_at), end}};
}

std::optional<std::pair<ast::Position, size_t>> ASTExtractor::extractPosition(std::string_view ast,
                                                                              const size_t start_parsing_at) const {
    const size_t coord_start = ast.find('[', start_parsing_at);
    if (coord_start == std::string_view::npos) {
        return {};
    }

    const size_t coord_end = ast.find(']', coord_start);
    if (coord_end == std::string_view::npos) {
        return {};
    }

    const auto coords = ast.substr(coord_start + 1, coord_end - coord_start - 1);
    const size_t comma = coords.find(',');
    if (comma == std::string_view::npos) {
        return {};
    }

    return {{ast::Position{ToSizeT(coords.substr(0, comma)), ToSizeT(coords.substr(comma + 2))}, coord_end}};
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

// clang-format off
std::optional<ast::Rect> ASTExtractor::GetNameLocation(std::string_view ast) const {
    return ExtractIdentifierASTFragment(ast)
        .and_then([&](const auto &p) {
            constexpr size_t start_parsing_at = 0;
            return ExtractRect(std::get<std::string_view>(p), start_parsing_at);
        })
        .and_then([&](const auto &p) {
            return std::optional<ast::Rect>{std::get<ast::Rect>(p)};
        });
}
// clang-format on

std::optional<ast::Rect> ASTExtractor::FindEnclosingClass(std::string_view ast, const ast::Rect &function_rect,
                                                          const size_t start_parsing_at) const {
    return findEnclosingParentEntityAST(ast, getDefinitionKeyword("class"), function_rect, start_parsing_at)
        .and_then([&](const auto &ast) { return GetNameLocation(ast); });
}

std::optional<std::string_view> ASTExtractor::FindEnclosingDecoratorAST(std::string_view ast,
                                                                        const ast::Rect &function_rect,
                                                                        const size_t start_parsing_at) const {
    return findEnclosingParentEntityAST(ast, getDefinitionKeyword("decorated"), function_rect, start_parsing_at);
}

std::optional<std::string_view> ASTExtractor::findEnclosingParentEntityAST(std::string_view ast,
                                                                           std::string_view marker,
                                                                           const ast::Rect &function_rect,
                                                                           const size_t start_parsing_at) const {
    std::optional<std::string_view> res;
    std::queue<std::pair<std::string_view, size_t>> q;

    const auto function_interval = ast::LinesInterval{function_rect};
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

                if (parent_interval.Contains(function_interval)) {
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
    const auto marker = getDefinitionKeyword("function");
    const auto pos = ast.find(marker, start_parsing_at);

    if (pos == std::string_view::npos) {
        return pos;
    } else {
        return pos + marker.length();
    }
}

// clang-format off
std::optional<ast::Rect> ASTExtractor::FirstFunctionDefinitionAfterDecorator(std::string_view ast) const {
    return ExtractFunctionDefinitionASTFragment(ast)
        .and_then([&](const auto &p) {
            return GetNameLocation(std::get<std::string_view>(p));
        });
}
// clang-format on

}  // namespace analyser::ast_extractor
