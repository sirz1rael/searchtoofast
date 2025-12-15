#ifndef SEARCHTOOFAST_HPP
#define SEARCHTOOFAST_HPP

#include "models/search_result.hpp"
#include "core/search/search_engine.hpp"
#include <filesystem>
#include <list>
#include <string>
#include <memory>

namespace searchtoofast {

/**
 * SearchToolFast - Main public API for the search tool
 *
 * Provides a simple interface for indexing files and searching for words.
 * Usage:
 *   SearchToolFast searcher("/path/to/root");
 *   searcher.index_directory("/path/to/search");
 *   auto results = searcher.search("keyword");
 */
class SearchTooFast {
private:
    std::unique_ptr<core::SearchEngine> search_engine;

public:
    /**
     * Initialize with a root directory
     * @param root_path Root directory for file indexing
     */
    explicit SearchTooFast(const std::filesystem::path &root_path);

    /**
     * Index all files in the specified directory
     * @param directory_path Directory to index
     */
    void index_directory(const std::filesystem::path &directory_path);

    /**
     * Search for a word in indexed files (case-insensitive)
     * @param word Word to search for
     * @return List of search results containing file path, line number, and line content
     */
    std::list<SearchResult> search(const std::string &word) const;
};

} // namespace searchtoofast

#endif // SEARCHTOOFAST_HPP
