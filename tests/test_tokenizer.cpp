#include "amg/tokenizer.hpp"
#include <gtest/gtest.h>

using amg::Tokenizer;

// Helper to run tokenize and return the vector
static std::vector<std::string> tks(const std::string& s) {
    return Tokenizer{}.tokenize(s);
}

TEST(Tokenizer, Empty) {
    EXPECT_TRUE(tks("").empty());
}

TEST(Tokenizer, SimpleWords) {
    EXPECT_EQ(tks("Hello world"),
              std::vector<std::string>({"hello", "world"}));
}

TEST(Tokenizer, Punctuation) {
    EXPECT_EQ(tks("foo,bar! baz."),
              std::vector<std::string>({"foo", "bar", "baz"}));
}

TEST(Tokenizer, StripAccentsAndCase) {
    EXPECT_EQ(tks("Café naïve"),
              std::vector<std::string>({"cafe", "naive"}));
}

TEST(Tokenizer, UnicodeWhitespace) {
    // U+00A0 is no-break space
    std::string s = "foo\u00A0bar";
    EXPECT_EQ(tks(s),
              std::vector<std::string>({"foo", "bar"}));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}







