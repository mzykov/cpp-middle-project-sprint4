#pragma once

#include <charconv>
#include <stdexcept>
#include <string>
#include <string_view>

inline size_t ToSizeT(std::string_view value) {
    size_t result{};
    auto [parse_end_ptr, error_code] = std::from_chars(value.begin(), value.end(), result);
    if (error_code != std::errc{} || parse_end_ptr != value.data() + value.size()) {
        throw std::invalid_argument("Cannot convert '" + std::string(value) + "' to integral");
    }
    return result;
}
