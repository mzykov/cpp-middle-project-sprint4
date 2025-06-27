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
    ExtractClassDefinitionASTFragment(const std::string &ast, size_t start_parsing_at = 0) const;

    std::optional<std::pair<std::string, size_t>>
    ExtractFunctionDefinitionASTFragment(const std::string &ast, size_t start_parsing_at = 0) const;

    std::optional<std::pair<std::string, size_t>>
    ExtractIdentifierASTFragment(const std::string &ast, size_t start_parsing_at = 0) const;

    std::optional<std::pair<std::string, size_t>>
    ExtractParametersASTFragment(const std::string &ast, size_t start_parsing_at = 0) const;

    std::optional<std::pair<std::string, size_t>>
    ExtractCommentASTFragment(const std::string &ast, size_t start_parsing_at = 0) const;

    std::optional<std::pair<std::string, size_t>>
    ExtractDecoratedDefinitionASTFragment(const std::string &ast, size_t start_parsing_at = 0) const;

    std::optional<std::pair<std::string, size_t>>
    ExtractDecoratorASTFragment(const std::string &ast, size_t start_parsing_at = 0) const;

    std::optional<std::pair<std::string, size_t>>
    extractASTFragment(const std::string &ast, const std::string &marker_with_one_opened_brace, size_t start_parsing_at) const;

    std::optional<std::pair<ast::Position, size_t>>
    extractPosition(const std::string &ast, size_t start_parsing_at) const;

    std::optional<std::pair<ast::Rect, size_t>>
    extractRect(const std::string &ast, size_t start_parsing_at) const;

    std::unordered_set<size_t>
    extractAllCommentLineNumbers(const std::string &ast, size_t start_parsing_at = 0) const;

    std::optional<ast::Rect> findEnclosingClass(const std::string &full_ast, const ast::Rect &func_rect) const;
    std::optional<ast::Rect> getNameLocation(const std::string &func_ast) const;

    size_t CountFirstLevelASTNodes(const std::string &ast) const;
    size_t CountNthLevelASTNodes(const std::string &ast, size_t level) const;
    // clang-format on
private:
};

}  // namespace analyser::ast_extractor
