#include "file.hpp"

#include <cstring>
#include <fstream>

namespace analyser::file {

File::File(std::string_view file_name) : name{file_name} {
    std::ifstream file(name);

    if (!file.is_open()) {
        throw std::invalid_argument("Can't open file " + name);
    }

    ast = readAST();
    source_lines = readSourceFile(file);
}

std::vector<std::string> File::readSourceFile(std::ifstream &file) {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

std::string File::readAST() try {
    std::string result;
    std::array<char, 256> buffer;

    const auto tree_sitter_cmd = getASTBuilberCmd(name);

    using PipePtr = std::unique_ptr<FILE, decltype([](FILE *pipe) {
                                        if (!pipe)
                                            return;

                                        const int status = pclose(pipe);
                                        if (WIFEXITED(status)) {
                                            const int exit_status = WEXITSTATUS(status);
                                            if (exit_status != 0) {
                                                throw std::runtime_error("Command failed with exit code " +
                                                                         std::to_string(exit_status));
                                            }
                                        } else {
                                            throw std::runtime_error("Command terminated abnormally");
                                        }
                                    })>;

    FILE *raw_pipe = popen(tree_sitter_cmd.c_str(), "r");
    if (!raw_pipe) {
        throw std::runtime_error("Failed to execute command: " + std::string{std::strerror(errno)});
    }
    PipePtr pipe(raw_pipe);

    while (fgets(buffer.data(), buffer.size(), pipe.get())) {
        result += buffer.data();
    }

    return result;
} catch (const std::exception &e) {
    throw std::runtime_error("Error while getting ast from " + name);
}

std::string File::getASTBuilberCmd(std::string_view file_name) {
    std::string cmd = "tree-sitter parse ";
    if (const auto env_var = std::getenv("TREE_SITTER_CONFIG_PATH"); env_var != nullptr) {
        cmd = cmd + "--config-path " + env_var;
    }
    cmd = cmd + file_name.data() + " 2>&1";
    return cmd;
}

}  // namespace analyser::file
