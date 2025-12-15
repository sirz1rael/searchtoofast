#ifndef SEARCH_ENGINE_HPP
#define SEARCH_ENGINE_HPP

#include "models/search_result.hpp"
#include "core/indexer/file_indexer.hpp"
#include <list>
#include <string>
#include <filesystem>

namespace searchtoofast::core {

class SearchEngine {
private:
    std::unique_ptr<FileIndexer> file_indexer;

public:
    explicit SearchEngine(const std::filesystem::path &root_path);

    /**
     * Index files in the specified directory
     * @param directory_path Path to directory to index
     */
    void index_directory(const std::filesystem::path &directory_path);

    /**
     * Search for a word in indexed files
     * @param word Word to search for (case-insensitive)
     * @return List of search results
     */
    std::list<::searchtoofast::SearchResult> find_word(const std::string &word) const;
};

} // namespace searchtoofast::core

#endif // SEARCH_ENGINE_HPP
