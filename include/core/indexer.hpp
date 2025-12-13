#ifndef INDEXER_HPP
#define INDEXER_HPP

#include <filesystem>
#include <set>
#include <unordered_map>
#include <list>

#pragma once

class Indexer {
private:
    std::set<std::string> folders_list;
    std::unordered_map<std::string, std::list<std::string>> folder_content;

    std::list<std::string> tokenize_path(const std::filesystem::path& path, char d = '/');
public:
    Indexer(const std::filesystem::path &path);
};

#endif
