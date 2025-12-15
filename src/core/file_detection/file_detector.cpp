#include "core/file_detection/file_detector.hpp"
#include <fstream>
#include <cstring>
#include <algorithm>

namespace searchtoofast::core {

bool FileDetector::is_binary(const std::filesystem::path& filepath,
                            size_t max_check) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) return false; // Can't read, assume text to be safe

    char buffer[1024];
    size_t bytes_to_check = std::min(max_check, static_cast<size_t>(1024));
    file.read(buffer, bytes_to_check);
    size_t bytes_read = file.gcount();

    if (bytes_read == 0) return false; // Empty file is text

    // Check for null bytes first (most common binary indicator, fastest check)
    if (memchr(buffer, 0, bytes_read) != nullptr) {
        // Has null byte - check if it's valid UTF-16 with BOM
        if (bytes_read >= 2) {
            unsigned char b0 = static_cast<unsigned char>(buffer[0]);
            unsigned char b1 = static_cast<unsigned char>(buffer[1]);

            // UTF-16 LE BOM
            if (b0 == 0xFF && b1 == 0xFE) {
                return !is_valid_utf16(buffer, bytes_read, true);
            }
            // UTF-16 BE BOM
            if (b0 == 0xFE && b1 == 0xFF) {
                return !is_valid_utf16(buffer, bytes_read, false);
            }
        }
        return true; // Has null byte without valid UTF-16 BOM = binary
    }

    // Check UTF-8 BOM
    if (bytes_read >= 3) {
        unsigned char b0 = static_cast<unsigned char>(buffer[0]);
        unsigned char b1 = static_cast<unsigned char>(buffer[1]);
        unsigned char b2 = static_cast<unsigned char>(buffer[2]);
        if (b0 == 0xEF && b1 == 0xBB && b2 == 0xBF) {
            return false; // UTF-8 with BOM is text
        }
    }

    return false; // No binary indicators found
}

bool FileDetector::is_valid_utf16(const char* data, size_t len, bool is_little_endian) {
    // UTF-16 validation: check for proper surrogate pair pairing
    if (len < 4) return true; // Too small to validate properly, assume valid
    if ((len & 1) != 0) return false; // Odd length is invalid

    const unsigned char* udata = reinterpret_cast<const unsigned char*>(data);

    for (size_t i = 0; i < len; i += 2) {
        uint16_t codepoint;
        if (is_little_endian) {
            codepoint = udata[i] | (udata[i+1] << 8);
        } else {
            codepoint = (udata[i] << 8) | udata[i+1];
        }

        // Check for surrogate ranges (0xD800-0xDFFF)
        if ((codepoint & 0xF800) == 0xD800) { // High surrogate range check
            if (codepoint >= 0xDC00) return false; // Low surrogate without high
            // High surrogate found, next must be low surrogate
            if (i + 2 >= len) return false;
            i += 2;
            uint16_t next_codepoint;
            if (is_little_endian) {
                next_codepoint = udata[i] | (udata[i+1] << 8);
            } else {
                next_codepoint = (udata[i] << 8) | udata[i+1];
            }
            // Verify it's a low surrogate (0xDC00-0xDFFF)
            if ((next_codepoint & 0xFC00) != 0xDC00) return false;
        }
    }
    return true;
}

} // namespace searchtoofast::core
