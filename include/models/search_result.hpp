#ifndef SEARCH_RESULT_HPP
#define SEARCH_RESULT_HPP

#include <vector>
#include <string>

namespace searchtoofast {

struct SearchResult {
    std::string word;
    int line_number;
    std::vector<std::string> line_content;
    std::string file_path;
};

} // namespace searchtoofast

#endif // SEARCH_RESULT_HPP
