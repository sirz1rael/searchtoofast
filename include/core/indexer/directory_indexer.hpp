#ifndef DIRECTORY_INDEXER_HPP
#define DIRECTORY_INDEXER_HPP

#include <filesystem>
#include <list>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>

namespace searchtoofast::core {

class DirectoryIndexer {
private:
    std::filesystem::path root_path;
    std::set<std::string> folders_list;
    std::unordered_map<std::string, std::list<std::string>> folder_content;

    std::list<std::string> tokenize_path(const std::filesystem::path& path);
    bool is_prefix_of(const std::list<std::string> &a, const std::list<std::string> &b);

public:
    explicit DirectoryIndexer(const std::filesystem::path &p);

    void index();

    std::vector<std::list<std::string>> get_requested_path_content(const std::filesystem::path &p);
};

} // namespace searchtoofast::core

#endif // DIRECTORY_INDEXER_HPP
