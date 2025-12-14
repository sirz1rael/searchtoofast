#include "core/indexer.hpp"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <list>
#include <string>

std::list<std::string> Indexer::tokenize_path(const std::filesystem::path& path, char d) {
    std::list<std::string> tokens;
    for (const auto& part : path) {
        if (!part.empty() && part != "/") {
            tokens.push_back(part.string());
        }
    }

    return tokens;
}

Indexer::Indexer(const std::filesystem::path &path) {
    try{
        for (const auto &_d : std::filesystem::recursive_directory_iterator(path))
            this->folders_list.insert(_d.path().string());

        for (const auto &d_entry : folders_list) {
            auto tokenized_entry = tokenize_path(d_entry);
            this->folder_content.insert({d_entry, tokenized_entry});
        }

        for (const auto &[folder_path, tokens] : this->folder_content) {
            std::cout << folder_path << " : ";
            for (const auto &token : tokens) {
                std::cout << token << ' ';
            }
            std::cout << std::endl;
        }
        std::cout << "Folder entries size: " << folders_list.size() << std::endl;
        auto r = get_requested_path_content("./CMakeFiles");

        for (const auto &a : r) {
            for (const auto &b : a) {
                std::cout << b << " ";
            }
            std::cout << std::endl;
        }

    } catch(const std::filesystem::filesystem_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

std::set<std::list<std::string>> Indexer::get_requested_path_content(const std::filesystem::path &p)  {
    // For example: got ./CMakeFiles
    auto tokenized_path = tokenize_path(p);
    // Res of previous: [".", "CMakeFiles"]

    std::set<std::list<std::string>> requested_path_content;

    for (const auto &[x, y] : folder_content) {
        // Checking all keys that contains ./build/CMakeFiles
        // How to check if key contains ./build/CMakeFiles?
        if (is_prefix_of(tokenized_path, y)) {
            requested_path_content.insert(std::list<std::string>{y});
        }
    }
    return requested_path_content;
}

bool Indexer::is_prefix_of(const std::list<std::string> &a, const std::list<std::string> &b) {
    if (a.size() > b.size()) return false;
    return std::equal(a.begin(), a.end(), b.begin());
}
