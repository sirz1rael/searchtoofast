#include "core/indexer/file_indexer.hpp"
#include "core/file_detection/file_detector.hpp"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>

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
        std::filesystem::path full_path;
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

std::list<std::string> FileIndexer::tokenize_line(const std::string& line) {
    static const std::regex word_regex("[A-Za-z0-9]+"); // Compile once, reuse for all calls
    std::list<std::string> tokenized_line;

    auto words_begin = std::sregex_iterator(line.begin(), line.end(), word_regex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator it = words_begin; it != words_end; ++it) {
        std::string data = it->str();
        // Convert to lowercase in-place
        for (auto& c : data) {
            c = std::tolower(static_cast<unsigned char>(c));
        }
        tokenized_line.push_back(std::move(data));
    }

    return tokenized_line;
}

std::list<std::list<std::string>> FileIndexer::read_and_tokenize_file(const std::filesystem::path& p) {
    if (!std::filesystem::exists(p))
        throw std::runtime_error("File does not exists: " + p.string());
    if (!std::filesystem::is_regular_file(p))
        throw std::runtime_error("Not a regular file: " + p.string());
    if (FileDetector::is_binary(p, 1024)) return {}; // don't try to tokenize binary files

    std::ifstream file(p);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + p.string());
    }
    std::string line;
    std::list<std::list<std::string>> lines; // preserves insertion order

    while (std::getline(file, line)) {
        lines.push_back(this->tokenize_line(line));
    }

    return lines;
}

} // namespace searchtoofast::core
