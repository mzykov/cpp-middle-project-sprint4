#pragma once

#include <utility>

namespace analyser::ast {

struct Position {
    size_t line = 0;
    size_t col = 0;
};

using Rect = std::pair<Position, Position>;

}  // namespace analyser::ast
