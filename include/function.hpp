#pragma once

#include "ast_extractor.hpp"
#include "file.hpp"

namespace analyser::function {

struct Function {
    std::string file_name;
    std::optional<std::string> class_name;
    std::string function_name;
    std::string ast;
    bool is_decorated = false;
};

class FunctionExtractor {
public:
    std::vector<Function> ProcessOneFile(const file::File &file) const;
    const ast_extractor::ASTExtractor &GetAstExtractor() const { return ast_extractor_; }

private:
    const ast_extractor::ASTExtractor ast_extractor_{};

    std::optional<std::pair<Function, size_t>> processASTFragment(const file::File &file, std::string_view ast,
                                                                  const size_t start_parsing_at = 0) const;

    std::string getNameFromSource(const ast::Rect &rect, const std::vector<std::string> &lines) const;
};

}  // namespace analyser::function
