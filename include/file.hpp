#pragma once

#include <memory>
#include <string>
#include <vector>

namespace analyser::file {

struct File {
    File(std::string_view file_name);
    std::string name;
    std::string ast;
    std::vector<std::string> source_lines;

private:
    std::vector<std::string> ReadSourceFile(std::ifstream &file);
    std::string GetAst();
};

}  // namespace analyser::file
