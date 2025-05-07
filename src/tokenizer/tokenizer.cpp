#include "amg/tokenizer.hpp"
#include <utf8proc.h>
#include <cwctype>
#include <string>
#include <vector>

// Unicode-aware tokenizer: leverages utf8proc for normalization and accent stripping.

//------------------------------------------------------------------------------
// strip_accents:
// - Normalizes input text to NFD form (decomposed)
// - Iterates codepoints, skipping all combining marks (Mn category)
// - Re-encodes remaining codepoints to UTF-8
//------------------------------------------------------------------------------
std::string strip_accents(const std::string& text) {
    // Perform NFD normalization using utf8proc
    utf8proc_uint8_t* normalized = utf8proc_NFD(reinterpret_cast<const utf8proc_uint8_t*>(text.c_str()));
    if (!normalized) {
        return text; // Return original if normalization fails
    }

    std::string result;
    utf8proc_int32_t codepoint;
    utf8proc_ssize_t index = 0;

    while (normalized[index]) {
        utf8proc_ssize_t size = utf8proc_iterate(&normalized[index], -1, &codepoint);
        if (size <= 0) break;

        utf8proc_category_t category = utf8proc_category(codepoint);
        if (category != UTF8PROC_CATEGORY_MN) {
            utf8proc_uint8_t buf[4];
            int len = utf8proc_encode_char(codepoint, buf);
            result.append(reinterpret_cast<char*>(buf), len);
        }

        index += size;
    }

    free(normalized);
    return result;
}

//------------------------------------------------------------------------------
// tokenize:
// - Strips accents via unicode normalization (strip_accents)
// - Casefolds each codepoint using utf8proc's category-aware lowercasing
// - Splits tokens on whitespace or punctuation (Unicode-aware)
//------------------------------------------------------------------------------
std::vector<std::string> tokenize(const std::string& text) {
    std::string cleaned = strip_accents(text);

    std::vector<std::string> tokens;
    std::string current;

    size_t i = 0;
    while (i < cleaned.size()) {
        utf8proc_int32_t cp = 0;
        utf8proc_ssize_t len = utf8proc_iterate(
            reinterpret_cast<const utf8proc_uint8_t*>(cleaned.data() + i),
            cleaned.size() - i,
            &cp
        );
        if (len <= 0) break;

        // Lowercase with utf8proc
        cp = utf8proc_tolower(cp);

        // Check for splitting on Unicode space or punctuation
        utf8proc_category_t cat = utf8proc_category(cp);
        if (cat == UTF8PROC_CATEGORY_ZS || cat == UTF8PROC_CATEGORY_PD ||
            cat == UTF8PROC_CATEGORY_PS || cat == UTF8PROC_CATEGORY_PE ||
            cat == UTF8PROC_CATEGORY_PI || cat == UTF8PROC_CATEGORY_PF ||
            cat == UTF8PROC_CATEGORY_PO) {
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