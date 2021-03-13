#include "parser.h"
#include "types.h"
#include "AST.h"
#include <iostream>
#include <memory>

Parser::Parser(Lexer lexer) {
    this->lexer = lexer;
    this->currentToken = this->lexer.getNextToken();
}

void Parser::error(std::string msg) {
    std::cout << msg << std::endl;
    exit(-1);
}

void Parser::eat(std::string type) {
    if(this->currentToken.type == type)
        this->currentToken = this->lexer.getNextToken();
    else
        this->error("Expected type: \"" + type + "\" got type: \"" + this->currentToken.type + '"');
}

std::shared_ptr<Value> Parser::value() {
    Token tok = this->currentToken;
    if(tok.type == STRING) {
        this->eat(STRING);
        auto stringPtr = std::make_shared<String>(String(tok.value));
        return stringPtr;
    }
    if(tok.type == NUMBER) {
        this->eat(NUMBER);
        auto numPtr = std::make_shared<Number>(Number(std::stof(tok.value)));
        return numPtr;
    }
    // first token type for object
    if(tok.type == LCURL) {
        std::shared_ptr<Value> objectPtr = this->object();
        return objectPtr;
    }
    // first token type for array
    if(tok.type == LSQUARE) {
        std::shared_ptr<Value> arrayPtr = this->array();
        return arrayPtr;
    }
    if(tok.type == BOOLEAN) {
        this->eat(BOOLEAN);
        std::shared_ptr<Boolean> stringPtr = std::make_shared<Boolean>(Boolean(tok.value == "true" ? true : false));
        return stringPtr;
    }
    if(tok.type == _NULL) {
        this->eat(_NULL);
        std::shared_ptr<Null> nullPtr = std::make_shared<Null>();
        return nullPtr;
    }

    throw std::runtime_error("Something went wrong in Parser::value()");
}

std::vector<std::shared_ptr<Value>> Parser::arrayList() {
    std::vector<std::shared_ptr<Value>> values;

    std::string tokType = this->currentToken.type;
    if(tokType == STRING || tokType == NUMBER || 
        tokType == LCURL || tokType == LSQUARE || 
            tokType == BOOLEAN || tokType == _NULL) {
        values.push_back(this->value());
        while(this->currentToken.type == COMMA) {
            this->eat(COMMA);
            for(auto elem : this->arrayList())
                values.push_back(elem);
        }
    }

    return values;
}

std::shared_ptr<Value> Parser::array() {
    this->eat(LSQUARE);
    auto values = this->arrayList();
    this->eat(RSQUARE);
    auto arrayPtr = std::make_shared<Array>(Array(values));
    // arrayPtr->values = values;
    return std::dynamic_pointer_cast<Value>(arrayPtr);
}

std::vector<std::shared_ptr<Assign>> Parser::objectList() {
    auto left = std::make_shared<String>(String(this->currentToken.value));
    this->eat(STRING);
    this->eat(COLON);
    std::vector<std::shared_ptr<Assign>> values = { std::make_shared<Assign>(Assign(left, this->value())) };
    while(this->currentToken.type == COMMA) {
        this->eat(COMMA);
        for(auto elem : this->objectList())
            values.push_back(elem);
    }
    return values;
}

std::shared_ptr<Value> Parser::object() {
    this->eat(LCURL);
    auto values = this->objectList();
    this->eat(RCURL);
    auto objPtr = std::make_shared<Object>();
    objPtr->values = values;
    return objPtr;
}

std::shared_ptr<AST> Parser::ast() {
    std::string tokType = this->currentToken.type;
    if(tokType == LCURL)
        return this->object();
    if(tokType == LSQUARE)
        return this->array();
    
    throw std::runtime_error("Invalid JSON");
}