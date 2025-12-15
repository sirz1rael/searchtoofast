#include "searchtoofast.hpp"
#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <root_directory> <search_directory> <word_to_find>" << std::endl;
        std::cerr << "Too few arguments called: " << argc << std::endl;
        return 1;
    }

    try {
        // Initialize searcher with root directory
        searchtoofast::SearchTooFast searcher(std::filesystem::path{argv[1]});

        // Index the specified directory
        searcher.index_directory(std::filesystem::path{argv[2]});

        // Search for the word
        auto results = searcher.search(std::string{argv[3]});

        if (results.empty()) {
            std::cout << "Word not found in any file." << std::endl;
            return 0;
        }

        for (const auto& result : results) {
            std::cout << "Word '" << result.word << "' found in file: " << result.file_path
                      << "\nIn line: '" << result.line_content << "'"
                      << " with line number: " << result.line_number << "\n" << std::endl;
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
