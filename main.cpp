#include "lexer.h"
#include "types.h"
#include "parser.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    std::string text, temp;
    if(!argv[1]) {
        std::cout << "Example usage: main.exe FILENAME" << std::endl;
        exit(1);
    }

    std::ifstream file(argv[1]);
    if(!file.is_open()) {
        std::cout << "Problem opening \"" << argv[1] << "\"" << std::endl;
        exit(-1);
    }
    while(std::getline(file, temp))
        text += temp + "\n";
    file.close();

    std::cout << text << std::endl;
    
    Lexer lexer(text);
    Parser parser(lexer);
    AST tree = parser.ast();
}