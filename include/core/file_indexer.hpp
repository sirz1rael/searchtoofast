#ifndef FILE_INDEXER_HPP
#define FILE_INDEXER_HPP

#include "directory_indexer.hpp"
#include <filesystem>
#include <memory>

class FileIndexer {
private:
    struct File{
        std::list<std::list<std::string>> file_content;
        std::filesystem::path file_path;

        bool operator<(const File& other) const {
            if (file_path != other.file_path)
                return file_path < other.file_path;
            return file_content != other.file_content;
        }
    };

    struct word_found_result {
        std::string word;
        int line_number;
        std::string line_content;
        std::string file_path;
    };

    std::unique_ptr<DirectoryIndexer> directory_indexer;

    std::set<File> files = {};

    std::list<std::list<std::string>> read_and_tokenize_file(const std::filesystem::path& p);
    std::list<std::string> tokenize_line(std::string line);
    static bool is_binary(const std::filesystem::path& filepath,
                         size_t max_check = 1024);
    static bool is_valid_utf16(const char* data, size_t len, bool is_little_endian);
public:
    FileIndexer(const std::filesystem::path &p);
    void request_folder_content(const std::filesystem::path &p);

    std::list<word_found_result> find_word_in_files(const std::string &word);
};

#endif
