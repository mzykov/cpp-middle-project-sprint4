#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <boost/functional/hash.hpp>

namespace analyser::ast {

struct Position {
    using ValueType = size_t;
    const ValueType line = 0;
    const ValueType col = 0;
    constexpr auto operator<=>(const Position &) const = default;

    constexpr friend std::size_t hash_value(const Position &p) {
        std::size_t seed = 0;
        boost::hash_combine(seed, p.line);
        boost::hash_combine(seed, p.col);
        return seed;
    }
};

using Rect = std::pair<Position, Position>;

struct LinesInterval {
    using ValueType = Position::ValueType;

    constexpr LinesInterval() = default;
    constexpr LinesInterval(const LinesInterval &other) : start_line(other.start_line), end_line(other.end_line) {}
    constexpr LinesInterval(const ValueType line) : start_line(line), end_line(line) {}
    constexpr LinesInterval(const ValueType start, const ValueType end) : start_line(start), end_line(end) {}
    constexpr LinesInterval(const Rect &r) : start_line(std::get<0>(r).line), end_line(std::get<1>(r).line) {}

    const ValueType start_line = 0;
    const ValueType end_line = 0;

    constexpr inline bool Contains(const LinesInterval &other) const {
        return start_line <= other.start_line && other.end_line <= end_line;
    }

    constexpr inline bool IsOneLine() const { return start_line == end_line; }
};

}  // namespace analyser::ast

namespace std {

template <>
struct hash<std::pair<analyser::ast::Position, analyser::ast::Position>> {
    std::size_t operator()(const analyser::ast::Rect &rect) const noexcept {
        std::size_t seed = 0;
        boost::hash_combine(seed, std::get<0>(rect));
        boost::hash_combine(seed, std::get<1>(rect));
        return seed;
    }
};

}  // namespace std
