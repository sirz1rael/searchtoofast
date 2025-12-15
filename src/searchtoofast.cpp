#include "searchtoofast.hpp"

namespace searchtoofast {

SearchTooFast::SearchTooFast(const std::filesystem::path &root_path)
    : search_engine(std::make_unique<core::SearchEngine>(root_path)) {}

void SearchTooFast::index_directory(const std::filesystem::path &directory_path) {
    search_engine->index_directory(directory_path);
}

std::list<SearchResult> SearchTooFast::search(const std::string &word) const {
    return search_engine->find_word(word);
}

} // namespace searchtoofast
