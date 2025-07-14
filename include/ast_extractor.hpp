#pragma once

#include <array>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "ast.hpp"

namespace analyser::ast_extractor {

// clang-format off
class ASTExtractor {
public:
    size_t
    CountFirstLevelASTNodes(std::string_view ast) const;

    size_t
    CountNthLevelASTNodes(std::string_view ast, const size_t level) const;

    std::unordered_set<size_t>
    ExtractAllCommentLineNumbers(std::string_view ast, const size_t start_parsing_at = 0) const;

    std::optional<std::pair<std::string, size_t>>
    ExtractASTFragment(std::string_view ast, std::string_view marker_started_with_one_opened_brace, const size_t start_parsing_at) const;

    std::optional<std::pair<std::string, size_t>>
    ExtractCommentASTFragment(std::string_view ast, const size_t start_parsing_at = 0) const;

    std::optional<std::pair<std::string, size_t>>
    ExtractFunctionDefinitionASTFragment(std::string_view ast, const size_t start_parsing_at = 0) const;

    std::optional<std::pair<std::string, size_t>>
    ExtractIdentifierASTFragment(std::string_view ast, const size_t start_parsing_at = 0) const;

    std::optional<std::pair<std::string, size_t>>
    ExtractParametersASTFragment(std::string_view ast, const size_t start_parsing_at = 0) const;

    std::optional<std::pair<ast::Rect, size_t>>
    ExtractRect(std::string_view ast, const size_t start_parsing_at = 0) const;

    std::optional<ast::Rect>
    FindEnclosingClass(std::string_view full_ast, const ast::Rect &function_rect, const size_t start_parsing_at = 0) const;

    std::optional<std::string>
    FindEnclosingDecoratorAST(std::string_view full_ast, const ast::Rect &function_rect, const size_t start_parsing_at = 0) const;

    size_t
    FindPositionAfterFunctionDefinition(std::string_view ast, const size_t start_parsing_at = 0) const;

    std::optional<ast::Rect>
    FirstFunctionDefinitionAfterDecorator(std::string_view decorated_ast) const;

    std::optional<ast::Rect>
    GetNameLocation(std::string_view function_ast) const;

    inline const auto &CyclomaticKeywords() const noexcept {
        static constexpr std::array<std::pair<std::string_view, size_t>, 12> cyclomatic_keywords {{
            {"(assert_statement", 1},
            {"(if_statement", 1},
            {"(elif_clause", 1},
            {"(else_clause", 1},
            {"(conditional_expression", 2}, // ternary
            {"(match_statement", 1},
            {"(case_clause", 1},
            {"(for_statement", 1},
            {"(while_statement", 1},
            {"(try_statement", 1},
            {"(except_clause", 1},
            {"(finally_clause", 1}
        }};
        return cyclomatic_keywords;
    }

private:
    std::optional<std::pair<ast::Position, size_t>>
    extractPosition(std::string_view ast, const size_t start_parsing_at = 0) const;

    std::optional<std::string>
    findEnclosingParentEntityAST(std::string_view full_ast, std::string_view marker, const ast::Rect &function_rect, const size_t start_parsing_at = 0) const;

    const auto &getDefinitionKeyword(std::string_view key) const {
        const std::unordered_map<std::string_view, std::string_view> definition_keywords {
            {"class", "(class_definition"},
            {"comment", "(comment"},
            {"decorated", "(decorated_definition"},
            {"function", "(function_definition"},
            {"identifier", "(identifier"},
            {"parameters", "(parameters"},
        };
        return definition_keywords.at(key);
    }
};
// clang-format on
}  // namespace analyser::ast_extractor
