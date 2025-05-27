#include "amg/tokenizer.hpp"
#include <utf8proc.h>

namespace amg {

//------------------------------------------------------------------------------
// strip_accents:
// - Performs NFD normalization and removes combining marks
//------------------------------------------------------------------------------
std::string Tokenizer::strip_accents(const std::string& text) {
    auto* normalized = utf8proc_NFD(
        reinterpret_cast<const utf8proc_uint8_t*>(text.c_str())
    );
    if (!normalized) {
        return text;
    }

    std::string result;
    utf8proc_int32_t codepoint;
    utf8proc_ssize_t index = 0;

    while (normalized[index]) {
        utf8proc_ssize_t size = utf8proc_iterate(&normalized[index], -1, &codepoint);
        if (size <= 0) break;

        // Skip non-spacing marks (Mn)
        if (utf8proc_category(codepoint) != UTF8PROC_CATEGORY_MN) {
            utf8proc_uint8_t buf[4];
            int len = utf8proc_encode_char(codepoint, buf);
            result.append(reinterpret_cast<char*>(buf), len);
        }
        index += size;Yes—tha
    }

    free(normalized);
    return result;
}

//------------------------------------------------------------------------------
// tokenize:
// - Strips accents, casefolds, and splits on Unicode whitespace/punctuation
//------------------------------------------------------------------------------
std::vector<std::string> Tokenizer::tokenize(const std::string& text) const {
    std::string cleaned = strip_accents(text);
    std::vector<std::string> tokens;
    std::string current;

    size_t i = 0;
    while (i < cleaned.size()) {
        utf8proc_int32_t cp = 0;
        utf8proc_ssize_t len = utf8proc_iterate(
            reinterpret_cast<const utf8proc_uint8_t*>(cleaned.data() + i),
            static_cast<utf8proc_ssize_t>(cleaned.size() - i),
            &cp
        );
        if (len <= 0) break;

        // Casefold
        cp = utf8proc_tolower(cp);
        auto cat = utf8proc_category(cp);

        // Split on whitespace or punctuation
        if (cat == UTF8PROC_CATEGORY_ZS ||
            (cat >= UTF8PROC_CATEGORY_PD && cat <= UTF8PROC_CATEGORY_PO))
        {
            if (!current.empty()) {
                tokens.push_back(std::move(current));
                current.clear();
            }
        } else {
            utf8proc_uint8_t buf[4];
            int n = utf8proc_encode_char(cp, buf);
            current.append(reinterpret_cast<char*>(buf), n);
        }
        i += len;
    }

    if (!current.empty()) {
        tokens.push_back(std::move(current));
    }
    return tokens;
}

} // namespace amg