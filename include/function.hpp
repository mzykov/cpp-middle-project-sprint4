#pragma once

#include <vector>

#include "ast_extractor.hpp"
#include "file.hpp"

namespace analyser::function {

struct Function {
    std::string filename;
    std::optional<std::string> class_name;
    std::string name;
    std::string ast;
};

class FunctionExtractor {
public:
    std::vector<Function> ProcessOneFile(const file::File &file) const;

private:
    ast_extractor::ASTExtractor extractor_;

    std::string getClassNameFromSource(const ast::Rect &class_rect, const std::vector<std::string> &lines) const;
    std::string getFunctionNameFromSource(const ast::Rect &func_rect, const std::vector<std::string> &lines) const;
};

}  // namespace analyser::function
