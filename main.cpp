#include "lexer.h"
#include "types.h"
#include <iostream>

int main() {
    Lexer lexer("[]{},\"asd\":");
    Token tok = lexer.getNextToken();
    while(!lexer.charIsNone) {
        std::cout << tok.value << std::endl;
        tok = lexer.getNextToken();
    }
}