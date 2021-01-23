#include "lexer.h"
#include "types.h"
#include <iostream>

Lexer::Lexer(std::string text) {
    this->text = text;
    this->currentChar = text[0];
}

void Lexer::advance() {
    this->pos++;
    if(this->pos > this->text.length()-1)
        this->charIsNone = true;
    else
        this->currentChar = this->text[this->pos];
}

Token Lexer::string() {
    std::string fullString;
    // char is already '"' so advance past it
    this->advance();
    while(this->currentChar != '"' && !this->charIsNone) {
        fullString += currentChar;
        this->advance();
    }
    // advance past last '"'
    this->advance();
    return Token(STRING, fullString);
}

Token Lexer::number() {
    std::string num;
    while(isdigit(this->currentChar) && !this->charIsNone) {
        num += this->currentChar;
        this->advance();
    }
    if(this->currentChar == '.') {
        num += '.';
        this->advance();
        while(isdigit(this->currentChar) && !this->charIsNone) {
            num += this->currentChar;
            this->advance();
        }
    }
    return Token(NUMBER, num);
}

Token Lexer::boolean() {
    std::string boolStr;
    while(isalpha(this->currentChar) && !this->charIsNone) {
        boolStr += this->currentChar;
        this->advance();
    }
    if(boolStr != "true" && boolStr != "false") {
        // text is neither true nor false and as such an error is thrown
        std::cout << "Invalid bool: " << boolStr << std::endl;
        exit(-1);
    }
    return Token(BOOLEAN, boolStr);
}

void Lexer::error(std::string msg) {
    std::cout << msg << std::endl;
    exit(-1);
}

Token Lexer::getNextToken() {
    while(!this->charIsNone) {
        char currentChar = this->currentChar;
        
        if(isspace(currentChar)) {
            this->advance();
            continue;
        }

        if(isalpha(currentChar))
            return this->boolean();

        if(isdigit(currentChar))
            return this->number();

        if(currentChar == '{') {
            this->advance();
            return Token(LCURL, "{");
        }
        if(currentChar == '}') {
            this->advance();
            return Token(RCURL, "}");
        }
        if(currentChar == '[') {
            this->advance();
            return Token(LSQUARE, "[");
        }
        if(currentChar == ']') {
            this->advance();
            return Token(RSQUARE, "]");
        }

        if(currentChar == '"')
            return this->string();
        if(currentChar == ':') {
            this->advance();
            return Token(COLON, ":");
        }
        if(currentChar == ',') {
            this->advance();
            return Token(COMMA, ",");
        }

        this->error("Invalid symbol: '" + std::string(1, currentChar) + "' at pos " + std::to_string(this->pos));
    }
    return Token(EOF, EOF);
}