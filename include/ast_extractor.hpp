#pragma once

#include <array>
#include <optional>
#include <string>
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

    const auto &CyclomaticKeywords() const noexcept { return cyclomatic_keywords_; }

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

private:
    static constexpr std::array<std::string_view, 12> cyclomatic_keywords_{{
        "(assert_statement",
        "(if_statement",
        "(elif_clause",
        "(else_clause",
        "(conditional_expression",
        "(match_statement",
        "(case_clause",
        "(for_statement",
        "(while_statement",
        "(try_statement",
        "(except_clause",
        "(finally_clause"
    }};

    std::optional<std::pair<ast::Position, size_t>>
    extractPosition(std::string_view ast, const size_t start_parsing_at = 0) const;

    std::optional<std::string>
    findEnclosingParentEntityAST(std::string_view full_ast, std::string_view marker, const ast::Rect &function_rect, const size_t start_parsing_at = 0) const;
};
// clang-format on
}  // namespace analyser::ast_extractor
