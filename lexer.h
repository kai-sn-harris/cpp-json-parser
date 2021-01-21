#pragma once
#include <string>

class Token {
public:
    std::string value, type;
    Token(std::string type, std::string value): value(value), type(type) {}
};

class Lexer {
public:
    int pos = 0;
    char currentChar;
    bool charIsNone = false;
    std::string text;

    Lexer(std::string text);

    void error(std::string msg);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           

    Token getNextToken();
    void advance();
    Token string();

};