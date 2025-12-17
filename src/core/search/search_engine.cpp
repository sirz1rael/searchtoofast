#include "core/search/search_engine.hpp"

namespace searchtoofast::core {

SearchEngine::SearchEngine(const std::filesystem::path &root_path)
    : file_indexer(std::make_unique<FileIndexer>(root_path)) {}

void SearchEngine::index_directory(const std::filesystem::path &directory_path) {
    file_indexer->request_folder_content(directory_path);
}

std::list<::searchtoofast::SearchResult> SearchEngine::find_word(const std::string &word) const {
    std::list<::searchtoofast::SearchResult> results;

    const auto& files = file_indexer->get_indexed_files();

    for (const auto& file : files) {
        std::string file_path_str = file.file_path.string();
        int line_number = 1;

        for (const auto& line_tokens : file.file_content) {
            // Check if word exists in this line
            bool word_found = false;
            for (const auto& token : line_tokens) {
                if (token == word) {
                    word_found = true;
                    break;
                }
            }

            // If word is found, reconstruct line content and add result
            if (word_found && !line_tokens.empty()) {
                std::string line_content;
                bool first = true;
                for (const auto& token : line_tokens) {
                    if (!first) line_content += " ";
                    line_content += token;
                    first = false;
                }

                results.push_back({word, line_number, line_content, file_path_str});
            }

            ++line_number;
        }
    }

    return results;
}

} // namespace searchtoofast::core
