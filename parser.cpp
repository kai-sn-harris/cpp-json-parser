#include "parser.h"
#include "types.h"
#include "AST.h"
#include <iostream>

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

Value* Parser::value() {
    Token tok = this->currentToken;
    if(tok.type == STRING) {
        this->eat(STRING);
        String* stringPtr = new String(tok.value);
        return stringPtr;
    }
    if(tok.type == NUMBER) {
        this->eat(NUMBER);
        Number* numberPtr = new Number(std::stof(tok.value));
        return numberPtr;
    }
    // first token type for object
    if(tok.type == LCURL) {
        Value* objectPtr = this->object();
        return objectPtr;
    }
    // first token type for array
    if(tok.type == LSQUARE) {
        Value* arrayPtr = this->array();
        return arrayPtr;
    }
    if(tok.type == BOOLEAN) {
        this->eat(BOOLEAN);
        Boolean* stringPtr = new Boolean(tok.value == "true" ? true : false);
        return stringPtr;
    }
    if(tok.type == _NULL) {
        this->eat(_NULL);
        Null* nullPtr = new Null();
        return nullPtr;
    }

    throw std::runtime_error("Something went wrong in Parser::value()");
}

std::vector<Value*> Parser::arrayList() {
    std::vector<Value*> values;

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

Value* Parser::array() {
    this->eat(LSQUARE);
    std::vector<Value*> values = this->arrayList();
    this->eat(RSQUARE);
    return new Array(values);
}

std::vector<Assign*> Parser::objectList() {
    String* left = new String(this->currentToken.value);
    this->eat(STRING);
    this->eat(COLON);
    std::vector<Assign*> values = { new Assign(left, this->value()) };
    while(this->currentToken.type == COMMA) {
        this->eat(COMMA);
        for(auto elem : this->objectList())
            values.push_back(elem);
    }
    return values;
}

Value* Parser::object() {
    this->eat(LCURL);
    std::vector<Assign*> values = this->objectList();
    this->eat(RCURL);
    Object* objPtr = new Object(values);
    return objPtr;
}

AST* Parser::ast() {
    std::string tokType = this->currentToken.type;
    if(tokType == LCURL)
        return this->object();
    if(tokType == LSQUARE)
        return this->array();
    
    throw std::runtime_error("Invalid JSON");
}