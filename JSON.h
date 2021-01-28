#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <any>
#include "lexer.h"
#include "types.h"
#include "parser.h"

class JSON {
public:
    std::map<std::string, std::any> variables;
    std::string text;
    JSON(std::string fileName);
    
    void generate();
    void visitNode(AST* node, std::map<std::string, std::any> &map);
    std::string visitString(Value* node);
    float visitNumber(Value* node);
    bool visitBoolean(Value* node);
    std::string visitNull(Value* node);

    std::string get(std::string key);
};