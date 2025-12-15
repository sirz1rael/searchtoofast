#include "core/file_indexer.hpp"
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Too few arguments called: " << argc << std::endl;
        return 1;
    }

    auto file_indexer = std::make_unique<FileIndexer>(std::filesystem::path(std::string(argv[1])));
    file_indexer->request_folder_content(std::filesystem::path(std::string(argv[2])));
    auto res = file_indexer->find_word_in_files(std::string(argv[3]));

    if(res.empty()) {
        std::cout << "Word not found in any file." << std::endl;
        return 0;
    }
    for (const auto& file : res) {
        std::cout << "Word '" << file.word << "' found in file: " << file.file_path << "\nIn line: '" << file.line_content << "'" << " with line number: " << file.line_number << "\n" << std::endl;
    }
    return 0;
}
