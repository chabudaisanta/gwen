#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <memory>

namespace test_utils {

// Helper to read file content into string
inline std::string read_file(const std::string& path) {
    std::ifstream ifs(path);
    if (!ifs.is_open()) return "";
    std::stringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}

// Trims trailing space/tabs from each line and removes trailing empty lines
inline std::string normalize_output(const std::string& text) {
    std::vector<std::string> lines;
    std::stringstream ss(text);
    std::string line;
    while (std::getline(ss, line)) {
        // Trim trailing whitespace
        while (!line.empty() && (line.back() == ' ' || line.back() == '\r' || line.back() == '\t' || line.back() == '\n')) {
            line.pop_back();
        }
        lines.push_back(line);
    }
    // Remove trailing empty lines
    while (!lines.empty() && lines.back().empty()) {
        lines.pop_back();
    }
    std::string result;
    for (size_t i = 0; i < lines.size(); ++i) {
        result += lines[i] + "\n";
    }
    return result;
}

// Executes an executable redirecting standard input from input_path
inline std::string run_problem_exe(const std::string& exe_path, const std::string& input_path) {
    // POSIX popen command: "exe_path < input_path 2>/dev/null"
    std::string cmd = exe_path + " < " + input_path + " 2>/dev/null";
    
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        return "ERROR: popen failed";
    }
    
    char buffer[128];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }
    return result;
}

} // namespace test_utils
