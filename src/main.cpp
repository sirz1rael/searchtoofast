#include "core/indexer.hpp"
#include <filesystem>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Too few arguments called: " << argc << std::endl;
    }

    Indexer i ((std::filesystem::path(argv[1])));
    
}
