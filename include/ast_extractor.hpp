#pragma once

#include <optional>
#include <string>
#include <unordered_set>

#include "ast.hpp"

namespace analyser::ast_extractor {

class ASTExtractor {
public:
    // clang-format off
    std::optional<std::pair<std::string, size_t>>
    ExtractClassDefinitionASTFragment(std::string_view ast, size_t start_parsing_at = 0) const;

    std::optional<std::pair<std::string, size_t>>
    ExtractFunctionDefinitionASTFragment(std::string_view ast, size_t start_parsing_at = 0) const;

    std::optional<std::pair<std::string, size_t>>
    ExtractIdentifierASTFragment(std::string_view ast, size_t start_parsing_at = 0) const;

    std::optional<std::pair<std::string, size_t>>
    ExtractParametersASTFragment(std::string_view ast, size_t start_parsing_at = 0) const;

    std::optional<std::pair<std::string, size_t>>
    ExtractCommentASTFragment(std::string_view ast, size_t start_parsing_at = 0) const;

    std::optional<std::pair<std::string, size_t>>
    ExtractDecoratedDefinitionASTFragment(std::string_view ast, size_t start_parsing_at = 0) const;

    std::optional<std::pair<std::string, size_t>>
    ExtractDecoratorASTFragment(std::string_view ast, size_t start_parsing_at = 0) const;

    std::optional<std::pair<std::string, size_t>>
    extractASTFragment(std::string_view ast, std::string_view marker_started_with_one_opened_brace, size_t start_parsing_at) const;

    std::optional<std::pair<ast::Position, size_t>>
    extractPosition(std::string_view ast, size_t start_parsing_at) const;

    std::optional<std::pair<ast::Rect, size_t>>
    extractRect(std::string_view ast, size_t start_parsing_at) const;

    std::unordered_set<size_t>
    extractAllCommentLineNumbers(std::string_view ast, size_t start_parsing_at = 0) const;

    std::optional<ast::Rect> findEnclosingClass(std::string_view full_ast, const ast::Rect &func_rect, size_t start_parsing_at = 0) const;
    std::optional<ast::Rect> getNameLocation(std::string_view func_ast) const;

    std::optional<std::string> findEnclosingDecoratorAST(std::string_view full_ast, const ast::Rect &func_rect, size_t start_parsing_at = 0) const;

    size_t findPositionAfterFunctionDefinition(std::string_view ast, size_t start_parsing_at = 0) const;
    std::optional<ast::Rect> firstFunctionDefinitionAfterDecorator(std::string_view decorated_ast) const;

    size_t CountFirstLevelASTNodes(std::string_view ast) const;
    size_t CountNthLevelASTNodes(std::string_view ast, size_t level) const;
    // clang-format on
private:
};

}  // namespace analyser::ast_extractor
