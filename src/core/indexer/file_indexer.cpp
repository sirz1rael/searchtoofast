#include "core/indexer/file_indexer.hpp"
#include "core/file_detection/file_detector.hpp"
#include "utils/tokenizer.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace searchtoofast::core {

FileIndexer::FileIndexer(const std::filesystem::path &p) {
    this->directory_indexer = std::make_unique<DirectoryIndexer>(p);
    directory_indexer->index(); // initiate directory indexing
}

void FileIndexer::request_folder_content(const std::filesystem::path &p) {
    auto path_content = directory_indexer->get_requested_path_content(p);

    std::cout << "\nContent of the path: " << p << ":" << std::endl;
    for (const auto& item : path_content) {
        // Build path more efficiently using filesystem::path operators
        std::filesystem::path full_path = "/";
        for (const auto& component : item) {
             full_path /= component;
        }

        // Check directory status once
        bool is_dir = std::filesystem::is_directory(full_path);
        std::cout << "Path: " << full_path.string() << " = "
                   << (is_dir ? "DIRECTORY" : "FILE") << std::endl;

        // Only process files, not directories
        if (!is_dir) {
            File file;
            file.file_content = read_and_tokenize_file(full_path);
            file.file_path = full_path;
            this->files.insert(std::move(file)); // Move the file object

            if (this->files.size() > 1500) {
                std::cout << "Reached 10,000 files limit, stopping indexing." << std::endl;
                break;
            }
        }
    }

    std::cout << files.size() << " files indexed\n" << std::endl;

    // Debug output
    for (const auto& file : files) {
        std::cout << "File: " << file.file_path << std::endl;
        for (const auto& line : file.file_content) {
            for (const auto& word : line) {
                std::cout << word << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "Lines indexed: " << file.file_content.size() << std::endl;
    }
}

std::vector<std::vector<std::string>> FileIndexer::read_and_tokenize_file(const std::filesystem::path& p) {
    if (!std::filesystem::exists(p))
        throw std::runtime_error("File does not exists: " + p.string());
    if (!std::filesystem::is_regular_file(p))
        throw std::runtime_error("Not a regular file: " + p.string());
    if (FileDetector::is_binary(p, 1024)) return {{"Binary file", "Can't read", p}}; // don't try to tokenize binary files

    std::ifstream file(p);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + p.string());
    }
    std::string line;
    std::vector<std::vector<std::string>> lines; // preserves insertion order

    while (std::getline(file, line)) {
        lines.push_back(utils::Tokenizer::tokenize(line));
    }

    return lines;
}

} // namespace searchtoofast::core
