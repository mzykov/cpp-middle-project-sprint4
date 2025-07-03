#pragma once

#include <memory>
#include <string>
#include <vector>

namespace analyser::file {

struct File {
    static inline const std::string command_prefix =
        "tree-sitter parse --config-path ~/.config/tree-sitter/config.json ";
    File(const std::string &file_name);
    std::string name;
    std::string ast;
    std::vector<std::string> source_lines;

private:
    std::vector<std::string> ReadSourceFile(std::ifstream &file);
    std::string GetAst(const std::string &file_name);
};

}  // namespace analyser::file
