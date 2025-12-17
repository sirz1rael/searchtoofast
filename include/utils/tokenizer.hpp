#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <regex>
#include <vector>
#include <string>

namespace searchtoofast::utils {

class Tokenizer {
private:
    static const std::regex word_regex;
public:
    static std::vector<std::string> tokenize(const std::string& line);
    static std::string make_lowercase(std::string& word);
};

}; // namespace searchtoofast::utils

#endif // TOKENIZER_HPP
