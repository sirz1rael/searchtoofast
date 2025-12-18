#include "core/indexer/directory_indexer.hpp"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <list>
#include <string>

namespace searchtoofast::core {

std::list<std::string> DirectoryIndexer::tokenize_path(const std::filesystem::path& path) {
    std::list<std::string> tokens;
    for (const auto& part : path) {
        if (!part.empty() && part != "/") {
            tokens.push_back(part.string());
        }
    }
    return tokens;
}

DirectoryIndexer::DirectoryIndexer(const std::filesystem::path &path)
    : root_path(path) {}

std::vector<std::list<std::string>> DirectoryIndexer::get_requested_path_content(const std::filesystem::path &p) {
    auto tokenized_path = tokenize_path(p);
    std::vector<std::list<std::string>> requested_path_content;

    for (const auto &[folder_path, tokens] : folder_content) {
        if (is_prefix_of(tokenized_path, tokens)) {
            requested_path_content.push_back(tokens);
        }
    }
    return requested_path_content;
}

bool DirectoryIndexer::is_prefix_of(const std::list<std::string> &a, const std::list<std::string> &b) {
    if (a.size() > b.size()) return false;
    return std::equal(a.begin(), a.end(), b.begin());
}

void DirectoryIndexer::index() {
    try {
        for (const auto &entry : std::filesystem::recursive_directory_iterator(root_path)) {
            this->folders_list.insert(entry.path().string());
        }

        for (const auto &folder_path : folders_list) {
            auto tokenized_entry = tokenize_path(folder_path);
            this->folder_content.insert({folder_path, tokenized_entry});
        }

        // for (const auto &[folder_path, tokens] : this->folder_content) {
        //     std::cout << folder_path << " : ";
        //     for (const auto &token : tokens) {
        //         std::cout << token << ' ';
        //     }
        //     std::cout << std::endl;
        // }
        std::cout << "Folder entries size: " << folders_list.size() << std::endl;
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

} // namespace searchtoofast::core
