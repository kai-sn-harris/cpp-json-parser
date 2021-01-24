#pragma once

#include "lexer.h"
#include "AST.h"

class Parser {
public:
    Lexer lexer;
    Token currentToken;
    Parser(Lexer lexer);

    void eat(std::string type);
    void error(std::string msg); 

    Value* object();
    std::vector<Assign*> objectList();

    Value* array();
    std::vector<Value*> arrayList();

    Value* value();

    AST ast();
};