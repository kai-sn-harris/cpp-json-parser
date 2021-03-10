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

Token Lexer::id() {
    std::string idStr;
    while(isalpha(this->currentChar) && !this->charIsNone) {
        idStr += this->currentChar;
        this->advance();
    }
    if(idStr != "true" && idStr != "false" && idStr != "null") {
        // text isn't true, false, or null and as such an error is thrown
        this->error("Invalid type: " + idStr);
    }
    if(idStr == "true" || idStr == "false")
        return Token(BOOLEAN, idStr);
    else
        return Token(_NULL, idStr);
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
            return this->id();

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
    return Token(_EOF, _EOF);
}