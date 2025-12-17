#include "utils/tokenizer.hpp"
#include <cctype>
#include <regex>

namespace searchtoofast::utils {

const std::regex Tokenizer::word_regex("[A-Za-z0-9]+");

std::vector<std::string> Tokenizer::tokenize(const std::string &line) {
    std::vector<std::string> tokenized_line;

    auto words_begin = std::sregex_iterator(line.begin(), line.end(), word_regex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator it = words_begin; it != words_end; ++it) {
        tokenized_line.push_back(it->str());
    }

    return tokenized_line;
}

std::string Tokenizer::make_lowercase(std::string &word) {
    for (auto& c : word)
        c = std::tolower(static_cast<unsigned char>(c));
    return word;
}

}; // namespace searchtoofast::utils
