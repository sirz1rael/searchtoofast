#ifndef FILE_INDEXER_HPP
#define FILE_INDEXER_HPP

#include "directory_indexer.hpp"
#include <filesystem>
#include <memory>

class FileIndexer {
private:
    struct File{
        std::set<std::list<std::string>> file_content;
        std::filesystem::path file_path;

        bool operator<(const File& other) const {
            if (file_path != other.file_path)
                return file_path < other.file_path;
            return file_content != other.file_content;
        }
    };
    std::unique_ptr<DirectoryIndexer> directory_indexer;

    std::set<File> files = {};

    std::set<std::list<std::string>> read_and_tokenize_file(const std::filesystem::path& p);
    std::list<std::string> tokenize_line(std::string line);
public:
    FileIndexer(const std::filesystem::path &p);
    void request_folder_content(const std::filesystem::path &p);
};

#endif
