#include "core/indexer.hpp"
#include <filesystem>
#include <iostream>
#include <list>

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
    } catch(const std::filesystem::filesystem_error& e) {
        std::cerr << e.what() << std::endl;
    }
}
