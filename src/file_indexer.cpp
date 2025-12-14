#include "core/file_indexer.hpp"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <regex>
#include <stdexcept>
#include <string>

FileIndexer::FileIndexer(const std::filesystem::path &p) {
    this->directory_indexer = std::make_unique<DirectoryIndexer>(p);
    directory_indexer->index(); // initiate directory indexing

    // after we got directories content, we need to define files in directories
}

void FileIndexer::request_folder_content(const std::filesystem::path &p) {
    auto path_content = directory_indexer->get_requested_path_content(p);

    std::cout << "\nContent of the path: " << p << ":" << std::endl;
    for (const auto& p : path_content) {
        auto res = std::accumulate(
            std::next(p.begin()),
            p.end(), p.front(),
            [](std::string a, const std::string& b) {
                return a + "/" + b;
            }
        );

        std::cout << "Path: " + res << " = " << (std::filesystem::is_directory(res) ? "DIRECTORY" : "FILE")  << std::endl;

        if (!std::filesystem::is_directory(res)) {
            auto file = File();
            file.file_content = read_and_tokenize_file(res);
            file.file_path = res;
            this->files.insert(file);
        }
    }

    std::cout << files.size() << " files indexed\n" << std::endl;

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

std::list<std::string> FileIndexer::tokenize_line(std::string line) {
    std::list<std::string> tokenized_line;

    std::regex word_regex("[A-Za-z0-9]+");
    auto words_begin = std::sregex_iterator(line.begin(), line.end(), word_regex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator it = words_begin; it != words_end; ++it) {
        auto data = it->str();
        std::transform(data.begin(), data.end(), data.begin(), [](unsigned char c) {return std::tolower(c);});
        tokenized_line.push_back(data);
    }

    return tokenized_line;
}

bool FileIndexer::is_binary(const std::filesystem::path& filepath,
                         size_t max_check) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) return false; // Can't read, assume text to be safe

    char buffer[1024];
    size_t bytes_to_check = std::min(max_check, static_cast<size_t>(1024));
    file.read(buffer, bytes_to_check);
    size_t bytes_read = file.gcount();

    // The GNU grep algorithm: binary if ANY null byte in first 1KB
    // (Except for UTF-16/32 with BOM, which we check first)

    // Check for UTF BOMs
    if (bytes_read >= 2) {
        // UTF-16 LE BOM
        if (buffer[0] == '\xFF' && buffer[1] == '\xFE') {
            // UTF-16 with BOM - check if it's valid text
            return !is_valid_utf16(buffer, bytes_read, true);
        }
        // UTF-16 BE BOM
        if (buffer[0] == '\xFE' && buffer[1] == '\xFF') {
            return !is_valid_utf16(buffer, bytes_read, false);
        }
    }

    // UTF-8 BOM
    if (bytes_read >= 3) {
        if (buffer[0] == '\xEF' && buffer[1] == '\xBB' && buffer[2] == '\xBF') {
            return false; // UTF-8 with BOM is text
        }
    }

    // Standard null-byte detection
    return std::any_of(buffer, buffer + bytes_read,
                          [](char c) { return c == 0; });
}

bool FileIndexer::is_valid_utf16(const char* data, size_t len, bool is_little_endian) {
        // Simplified UTF-16 validation
        // In practice, you'd want a more complete check
        if (len % 2 != 0) return false;

        for (size_t i = 0; i < len; i += 2) {
            uint16_t codepoint;
            if (is_little_endian) {
                codepoint = static_cast<uint8_t>(data[i]) |
                           (static_cast<uint8_t>(data[i+1]) << 8);
            } else {
                codepoint = (static_cast<uint8_t>(data[i]) << 8) |
                            static_cast<uint8_t>(data[i+1]);
            }

            // Check for invalid UTF-16 ranges
            if (codepoint >= 0xD800 && codepoint <= 0xDFFF) {
                // Surrogate pair - check if properly paired
                if (codepoint >= 0xDC00) return false; // Low surrogate without high
                i += 2; // Skip next char (should be low surrogate)
                if (i >= len) return false;
            }
        }
        return true;
    }

std::set<std::list<std::string>> FileIndexer::read_and_tokenize_file(const std::filesystem::path& p) {
    if (!std::filesystem::exists(p))
        throw std::runtime_error("File does not exists: " + p.string());

    if (!std::filesystem::is_regular_file(p))
        throw std::runtime_error("Not a regular file: " + p.string());

    auto file_size = std::filesystem::file_size(p);
    if (file_size == 0) return {};

    std::ifstream file(p);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + p.string());
    }

    std::set<std::list<std::string>> lines;
    std::string line;

    while (std::getline(file, line)) {
        lines.insert(this->tokenize_line(line));
    }

    return lines;
}
