#ifndef FILE_INDEXER_HPP
#define FILE_INDEXER_HPP

#include "core/indexer/directory_indexer.hpp"
#include <filesystem>
#include <memory>
#include <list>
#include <set>
#include <string>

namespace searchtoofast::core {

class FileIndexer {
private:
    struct File {
        std::list<std::list<std::string>> file_content;
        std::filesystem::path file_path;

        bool operator<(const File& other) const {
            if (file_path != other.file_path)
                return file_path < other.file_path;
            return file_content != other.file_content;
        }
    };

    std::unique_ptr<DirectoryIndexer> directory_indexer;
    std::set<File> files = {};

    std::list<std::list<std::string>> read_and_tokenize_file(const std::filesystem::path& p);
    std::list<std::string> tokenize_line(const std::string& line);

public:
    FileIndexer(const std::filesystem::path &p);
    void request_folder_content(const std::filesystem::path &p);
    const std::set<File>& get_indexed_files() const { return files; }
};

} // namespace searchtoofast::core

#endif // FILE_INDEXER_HPP
