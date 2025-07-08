#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace analyser::ast {

struct Position {
    using ValueType = size_t;
    ValueType line = 0;
    ValueType col = 0;
    auto operator<=>(const Position &) const = default;
};

using Rect = std::pair<Position, Position>;

struct LinesInterval {
    using ValueType = Position::ValueType;

    LinesInterval() = default;
    LinesInterval(const LinesInterval &other) : start_line(other.start_line), end_line(other.end_line) {}
    LinesInterval(const ValueType line) : start_line(line), end_line(line) {}
    LinesInterval(const ValueType start, const ValueType end) : start_line(start), end_line(end) {}
    LinesInterval(const Rect &r) : start_line(std::get<0>(r).line), end_line(std::get<1>(r).line) {}

    LinesInterval &operator=(const LinesInterval &other) {
        start_line = other.start_line;
        end_line = other.end_line;
        return *this;
    }

    ValueType start_line = 0;
    ValueType end_line = 0;

    inline bool Contains(const LinesInterval &other) const {
        return start_line <= other.start_line && other.end_line <= end_line;
    }

    inline bool IsOneLine() const { return start_line == end_line; }
};

struct ASTreeNode {
    ASTreeNode() = default;
    ASTreeNode(const std::string &node_name) : name{node_name}, rect{} {}
    ASTreeNode(const std::string &node_name, Rect node_rect) : name{node_name}, rect{node_rect} {}
    std::string name;
    Rect rect;
    std::vector<std::unique_ptr<ASTreeNode>> children;
};

std::pair<std::string, size_t> extractNodeName(std::string_view s, size_t start_parsing_at);

}  // namespace analyser::ast
