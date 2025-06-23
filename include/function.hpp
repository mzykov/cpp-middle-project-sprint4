#pragma once

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "file.hpp"

namespace fs = std::filesystem;
namespace rv = std::ranges::views;
namespace rs = std::ranges;

namespace analyser::function {

struct Function {
    std::string filename;
    std::optional<std::string> class_name;
    std::string name;
    std::string ast;
};

struct Position {
    size_t line = 0;
    size_t col = 0;
};

using Rect = std::pair<Position, Position>;

struct FunctionExtractor {
    std::vector<Function> Get(const file::File &file) const;

private:
    std::pair<Position, size_t> extractPosition(const std::string &ast, size_t start_parsing_at) const;
    std::pair<Rect, size_t> extractRect(const std::string &ast, size_t start_parsing_at) const;

    std::optional<Rect> findEnclosingClass(const std::string &full_ast, const Rect &func_rect) const;
    std::string getClassNameFromSource(const Rect &class_rect, const std::vector<std::string> &lines) const;

    Rect getNameLocation(const std::string &function_ast) const;
    std::string getNameFromSource(const Rect &func_rect, const std::vector<std::string> &lines) const;
};

}  // namespace analyser::function
