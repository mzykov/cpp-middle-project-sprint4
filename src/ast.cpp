#include "ast.hpp"

namespace analyser::ast {

std::pair<std::string, size_t> extractNodeName(std::string_view s, size_t start_parsing_at) {
    std::string_view allowed_chars{"abcdefghigklmnopqrstuvwxyz_"};
    auto continue_parsing_at = s.find_first_not_of(allowed_chars, start_parsing_at);

    if (continue_parsing_at == std::string::npos) {
        return {"", std::string::npos};
    }

    auto name = std::string{s.substr(start_parsing_at, continue_parsing_at - start_parsing_at)};

    return {name, continue_parsing_at};
}

size_t buildASTreeRecursive(std::string_view s, ASTreeNode *parent_node, size_t start_parsing_at) {
    // auto [node_name, continue_parsing_at] = extractNodeName(s, start_parsing_at);
    std::string node_name = "test";
    size_t continue_parsing_at = 0;

    parent_node->children.emplace_back(std::make_unique<ASTreeNode>(node_name));

    return continue_parsing_at;
}

std::unique_ptr<ASTreeNode> buildASTree(const std::string &s) {
    auto start_parsing_at = s.find_first_of("(");

    if (start_parsing_at == std::string::npos) {
        return nullptr;
    }

    return nullptr;
}

}  // namespace analyser::ast
