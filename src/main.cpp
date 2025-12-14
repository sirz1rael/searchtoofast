#include "core/file_indexer.hpp"
#include <filesystem>
#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Too few arguments called: " << argc << std::endl;
        return 1;
    }

    auto file_indexer = std::make_unique<FileIndexer>(std::filesystem::path(argv[1]));
    file_indexer->request_folder_content(std::filesystem::path(argv[2]));
    return 0;
}
