// src/main.cpp
#include <iostream>
#include "amg/tokenizer.hpp"

int main() {
    std::string input = u8"¡Niños, niñas…漢字 Ωmega";
    amg::Tokenizer tokenizer;                     // create an instance
    auto tokens = tokenizer.tokenize(input);      // call the member fn

    for (const auto& t : tokens) {
        std::cout << t << '\n';
    }
    return 0;
}
