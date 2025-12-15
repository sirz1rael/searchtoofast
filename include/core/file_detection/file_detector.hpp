#ifndef FILE_DETECTOR_HPP
#define FILE_DETECTOR_HPP

#include <filesystem>
#include <cstddef>

namespace searchtoofast::core {

class FileDetector {
public:
    /**
     * Determines if a file is binary or text
     * @param filepath Path to the file to check
     * @param max_check Maximum bytes to check (default 1024)
     * @return true if file is binary, false if text
     */
    static bool is_binary(const std::filesystem::path& filepath,
                         size_t max_check = 1024);

private:
    /**
     * Validates UTF-16 encoding
     * @param data Pointer to file data
     * @param len Length of data to check
     * @param is_little_endian Whether data is little-endian
     * @return true if valid UTF-16, false otherwise
     */
    static bool is_valid_utf16(const char* data, size_t len, bool is_little_endian);
};

} // namespace searchtoofast::core

#endif // FILE_DETECTOR_HPP
