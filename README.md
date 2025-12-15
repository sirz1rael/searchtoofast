# SearchToolFast ⚡

> A high-performance, modular C++20 file search tool with professional architecture

A fast, efficient command-line utility for searching text across files in a directory tree. Built with modern C++20, clean architecture principles, and optimized for performance.

## ✨ Features

- **Fast File Search** - Quickly search for words across multiple files
- **Binary File Detection** - Automatically skips binary files
- **UTF-8 & UTF-16 Support** - Handles multiple text encodings
- **Case-Insensitive Search** - Find matches regardless of case
- **Detailed Results** - Shows file path, line number, and line content
- **Modular Architecture** - Clean separation of concerns, easy to extend

## 🏗️ Architecture

The project follows a layered architecture with clear separation of concerns:

```
┌─────────────────────────────────────┐
│   SearchTooFast (Public API)       │
├─────────────────────────────────────┤
│   SearchEngine (Orchestration)      │
├─────────────────────────────────────┤
│  FileIndexer  │  DirectoryIndexer   │
├─────────────────────────────────────┤
│   FileDetector (Utilities)          │
└─────────────────────────────────────┘
```

### Components

- **SearchTooFast** - Main public API, simple interface
- **SearchEngine** - Search orchestration and coordination
- **FileIndexer** - File reading and tokenization
- **DirectoryIndexer** - Directory tree traversal
- **FileDetector** - Binary/text file detection, encoding validation

## 📋 Requirements

- **C++20 or later**
- **CMake 3.10+**
- **Linux/macOS/Windows** (cross-platform compatible)

## 🚀 Quick Start

### Build

```bash
cd searchtoofast
mkdir build
cd build
cmake ..
make
```

### Usage

```bash
./bin/searchtoofast <root_directory> <search_directory> <word_to_find>
```

**Example:**
```bash
./bin/searchtoofast . ./src main
```

This searches for the word "main" in all files under `./src` directory.

### Output

```
Content of the path: ./src:
Path: ./src/main.cpp = FILE
Path: ./src/searchtoofast.cpp = FILE
Path: ./src/core/indexer/file_indexer.cpp = FILE
...
3 files indexed

Word 'main' found in file: ./src/main.cpp
In line: '#include "searchtoofast.hpp"
int main() {'
 with line number: 6
```

## 💻 Usage as Library

You can also use SearchTooFast as a library in your own C++ projects:

```cpp
#include "searchtoofast.hpp"
#include <iostream>

int main() {
    // Create searcher
    searchtoofast::SearchTooFast searcher("/path/to/root");

    // Index a directory
    searcher.index_directory("/path/to/search");

    // Search for a word
    auto results = searcher.search("keyword");

    // Process results
    for (const auto& result : results) {
        std::cout << "Found in: " << result.file_path
                  << " at line " << result.line_number << std::endl;
        std::cout << "Content: " << result.line_content << std::endl;
    }

    return 0;
}
```

## 📁 Project Structure

```
searchtoofast/
├── include/
│   ├── searchtoofast.hpp              # Main public API
│   ├── models/
│   │   └── search_result.hpp          # Data structures
│   └── core/
│       ├── indexer/
│       │   ├── file_indexer.hpp
│       │   └── directory_indexer.hpp
│       ├── file_detection/
│       │   └── file_detector.hpp
│       └── search/
│           └── search_engine.hpp
├── src/
│   ├── main.cpp                       # CLI application
│   ├── searchtoofast.cpp              # API implementation
│   └── core/
│       ├── indexer/
│       ├── file_detection/
│       └── search/
├── tests/                             # Unit tests (ready for implementation)
├── CMakeLists.txt
├── README.md
├── RESTRUCTURING.md
└── LICENSE
```

## 🔧 Building with Options

### Debug Build
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

### Release Build (Optimized)
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

### Enable Tests
```bash
cmake -DBUILD_TESTS=ON ..
make
```

## 📊 Performance

SearchToolFast is optimized for performance:

- **Static regex compilation** - Compiled once, reused for all lines
- **Efficient path handling** - Minimal string copies
- **Binary file skipping** - Doesn't process non-text files
- **Single pass indexing** - Files read once during indexing
- **Memory efficient** - Uses move semantics and smart pointers

## 🛠️ Development

### Adding Tests

Create unit tests in the `tests/` directory:

```bash
# tests/unit/test_file_indexer.cpp
#include <gtest/gtest.h>
#include "core/indexer/file_indexer.hpp"

TEST(FileIndexer, IndexesFiles) {
    // Test implementation
}
```

Then update `CMakeLists.txt` to include test targets.

## 🚦 API Reference

### SearchToolFast

```cpp
namespace searchtoofast {

class SearchTooFast {
public:
    // Constructor
    explicit SearchTooFast(const std::filesystem::path &root_path);

    // Index a directory
    void index_directory(const std::filesystem::path &directory_path);

    // Search for a word
    std::list<SearchResult> search(const std::string &word) const;
};

} // namespace searchtoofast
```

### SearchResult

```cpp
namespace searchtoofast {

struct SearchResult {
    std::string word;           // The word that was searched
    int line_number;            // Line number where found (1-indexed)
    std::string line_content;   // Full content of the line
    std::string file_path;      // Path to the file containing the match
};

} // namespace searchtoofast
```

## 🔍 Supported File Types

- **Text Files**: All plain text files (`.txt`, `.cpp`, `.hpp`, `.c`, `.h`, `.md`, etc.)
- **UTF-8 Encoded**: Standard text files with UTF-8 encoding
- **UTF-8 with BOM**: UTF-8 files with Byte Order Mark
- **UTF-16 Encoded**: UTF-16 LE and UTF-16 BE with BOM

## ⚠️ Limitations

- Currently searches one directory at a time
- Does not follow symbolic links
- Search is case-insensitive only (case-sensitive search coming soon)
- No regex pattern support (exact word matching only)

## 🔜 Future Enhancements

- [ ] Inverted index for O(1) lookups
- [ ] Parallel file processing
- [ ] Regex pattern support
- [ ] Case-sensitive search option
- [ ] Result caching
- [ ] Web API interface
- [ ] GUI application
- [ ] Configuration file support

## 📝 Examples

### Search in Source Code

```bash
./bin/searchtoofast . ./src "include"
```

Find all files that contain the word "include".

### Search in Project

```bash
./bin/searchtoofast /home/user/project ./src "TODO"
```

Find all TODO comments in source files.

### Search in Documentation

```bash
./bin/searchtoofast . ./docs "API"
```

Search for API references in documentation.

## 🤝 Contributing

Contributions are welcome! The codebase is organized for easy extension:

1. Add new components in `src/core/` following existing patterns
2. Create corresponding headers in `include/core/`
3. Update `CMakeLists.txt` with new source files
4. Add unit tests in `tests/`

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 📚 Documentation

- **[API Reference](#-api-reference)** - Complete API documentation
- **Code Comments** - Comprehensive inline documentation

## 🐛 Known Issues

- None currently reported

## 📞 Support

For issues, questions, or suggestions, please open an issue on the project repository.

## 🎯 Roadmap

### v1.0 (Current)
- ✅ Basic file search functionality
- ✅ Binary file detection
- ✅ UTF-8 and UTF-16 support
- ✅ Professional architecture

### v1.1 (Planned)
- [ ] Inverted index implementation
- [ ] Parallel file processing
- [ ] Result caching

### v2.0 (Future)
- [ ] Regex support
- [ ] Web API
- [ ] GUI application
- [ ] Performance benchmarks

---

**Made with ❤️ for fast file searching**
