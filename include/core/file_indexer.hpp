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
    static bool is_binary(const std::filesystem::path& filepath,
                         size_t max_check = 1024);
    static bool is_valid_utf16(const char* data, size_t len, bool is_little_endian);
public:
    FileIndexer(const std::filesystem::path &p);
    void request_folder_content(const std::filesystem::path &p);
};

#endif
