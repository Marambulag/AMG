// src/main.cpp
#include <iostream>
#include "amg/tokenizer.hpp"

int main() {
    std::string input = u8"Hola, mUndo! ¿Cómo estás?"; 
    amg::Tokenizer tokenizer;                    
    auto tokens = tokenizer.tokenize(input);     

    for (const auto& t : tokens) {
        std::cout << t << '\n';
    }
    return 0;
}
