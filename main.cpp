#include "lexer.h"
#include "types.h"
#include <iostream>

int main() {
    Lexer lexer("[]{},\"asd\": true false 5 6.7 3.1415 ");
    Token tok = lexer.getNextToken();
    while(!lexer.charIsNone) {
        std::cout << tok.value << std::endl;
        tok = lexer.getNextToken();
    }
}