#pragma once

#include <memory>
#include <string>
#include <vector>

namespace analyser::file {

struct File {
    File(std::string_view file_name);
    const std::string name;
    std::string ast;
    std::vector<std::string> source_lines;

private:
    std::vector<std::string> readSourceFile(std::ifstream &file);
    std::string readAST();
    std::string getASTBuilberCmd(std::string_view file_name);
};

}  // namespace analyser::file
