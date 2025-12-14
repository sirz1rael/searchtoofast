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
        std::cout << std::endl;
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
