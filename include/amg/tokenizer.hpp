#ifndef AMG_TOKENIZER_HPP
#define AMG_TOKENIZER_HPP

#include <string>
#include <vector>

namespace amg {

class Tokenizer {
public:
    // match exactly the definition in tokenizer.cpp
    std::vector<std::string> tokenize(const std::string& text) const;

private:
    static std::string strip_accents(const std::string& text);
};

} // namespace amg

#endif // AMG_TOKENIZER_HPP
// tokenizer.hpp
