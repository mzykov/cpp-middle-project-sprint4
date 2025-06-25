#pragma once

#include <optional>
#include <string>

#include "ast.hpp"

namespace analyser::ast_extractor {

struct ASTExtractor {
    std::pair<ast::Position, size_t> extractPosition(const std::string &ast, size_t start_parsing_at) const;
    std::pair<ast::Rect, size_t> extractRect(const std::string &ast, size_t start_parsing_at) const;
    std::optional<std::pair<std::string, size_t>> extractASTFragment(const std::string &ast,
                                                                     const std::string &marker_with_one_opened_brace,
                                                                     size_t start_parsing_at) const;
    std::optional<ast::Rect> findEnclosingClass(const std::string &full_ast, const ast::Rect &func_rect) const;
    std::optional<ast::Rect> getNameLocation(const std::string &func_ast) const;
};

}  // namespace analyser::ast_extractor
