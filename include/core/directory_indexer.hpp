#ifndef INDEXER_HPP
#define INDEXER_HPP

#include <filesystem>
#include <set>
#include <unordered_map>
#include <list>

class DirectoryIndexer {
private:
    std::filesystem::path current_working_path;
    std::set<std::string> folders_list;
    std::unordered_map<std::string, std::list<std::string>> folder_content;

    std::list<std::string> tokenize_path(const std::filesystem::path& path, char d = '/');
    bool is_prefix_of(const std::list<std::string> &a, const std::list<std::string> &b);
public:
    DirectoryIndexer(const std::filesystem::path &path);
    std::set<std::list<std::string>> get_requested_path_content(const std::filesystem::path &p);
    void index();
};

#endif
