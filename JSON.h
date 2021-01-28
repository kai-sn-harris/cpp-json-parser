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

    template <class T>
    T get(std::string key) {
        std::vector<std::string> keys;
        std::string word;
        for(int i = 0; i < key.size(); i++) {
            if(key[i] == '.') {
                keys.push_back(word);
                word = "";
            } else word += key[i];
        }
        // last word
        keys.push_back(word);

        std::any value;
        for(int i = 0; i < keys.size(); i++) {
            if(i == 0) value = this->variables[keys[0]];
            // ignoring arrays for now
            else if(i > 0 && i < keys.size()-2) value = std::any_cast<std::map<std::string, std::any>>(value)[keys[i]];
            else value = std::any_cast<T>(std::any_cast<std::map<std::string, std::any>>(value)[keys[i]]);
        }
        return std::any_cast<T>(value);
    }
};