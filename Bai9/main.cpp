#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <unordered_set>

namespace fs = std::filesystem;

bool hasInvalidChars(const std::string& filename) {
    static const std::unordered_set<char> invalidChars {
        '<', '>', ':', '"', '/', '\\', '|', '?', '*'
    };
    for (char c : filename) {
        if (invalidChars.count(c)) return true;
    }
    return false;
}

void printPathInfo(const std::string& pathStr) {
    try {
        fs::path p(pathStr);

        if (p.empty() || !p.has_filename()) {
            throw std::invalid_argument("Invalid path syntax or missing filename.");
        }

        std::string folder = p.parent_path().string();
        if (folder.empty()) {
            throw std::invalid_argument("Path does not contain a valid folder.");
        }

        std::string filename = p.filename().string();
        if (hasInvalidChars(filename)) {
            throw std::invalid_argument("Filename contains invalid characters.");
        }

        std::string extension;
        if (p.has_extension()) {
            extension = p.extension().string();
            if (!extension.empty() && extension.front() == '.') {
                extension.erase(0, 1);
            }
        } else {
            extension = "(none)";
        }

        std::cout << "Folder: "   << folder    << "\n"
                  << "Filename: " << filename  << "\n"
                  << "Extension: " << extension << "\n\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n\n";
    }
}

int main() {
    // Valid Windows path
    printPathInfo("C:\\Windows\\Dev\\.data.rar");

    // Valid Linux/macOS path
    printPathInfo("/usr/local/bin/script.sh");

    printPathInfo("script.sh");

    // Invalid filename (contains '|')
    printPathInfo("C:\\Windows\\Dev\\bad|name.txt");

    // Missing filename
    printPathInfo("C:\\Windows\\Dev\\");
}
