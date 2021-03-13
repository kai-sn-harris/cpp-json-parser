#pragma once

#include "lexer.h"
#include "AST.h"
#include <memory>

class Parser {
public:
    Lexer lexer;
    Token currentToken;
    Parser(Lexer lexer);

    void eat(std::string type);
    void error(std::string msg); 

    std::shared_ptr<Value> object();
    std::vector<std::shared_ptr<Assign>> objectList();

    std::shared_ptr<Value> array();
    std::vector<std::shared_ptr<Value>> arrayList();

    std::shared_ptr<Value> value();

    std::shared_ptr<AST> ast();
};