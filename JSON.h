#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <any>
#include <exception>
#include <type_traits>
#include "lexer.h"
#include "types.h"
#include "parser.h"

class JSON {
public:
    std::map<std::string, std::any> variables;
    std::vector<std::any> rootArray;
    std::string text;
    JSON(std::string fileName);

    template <class T>
    T get(std::string key) {
        std::string raw_key = key;
        bool isRootArray = false;
        if(key.size() > 4 && (key[0] == '(' && key[1] == 'a' && key[2] == 'r' && key[3] == ')')) {
            isRootArray = true;
            key.erase(0, 4);
        }
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
            if(i == 0) {
                if(!isRootArray) {
                    if(this->variables[keys[0]].has_value()) value = this->variables[keys[0]];
                    else throw std::runtime_error("There is no root object. Add \"(ar)\" to your key to read from the root array array.");
                }
                else {
                    if(this->rootArray.size() != 0) value = this->rootArray[std::stoi(keys[0])];
                    else throw std::runtime_error("There is no root array. Remove \"(ar)\" from your key to read from the root object.");
                }
            }
            else {
                try {
                    // for object
                    value = std::any_cast<std::map<std::string, std::any>>(value)[keys[i]];
                } catch(std::bad_any_cast const&) {
                    // for array
                    value = std::any_cast<std::vector<std::any>>(value)[std::stoi(keys[i])];
                }
            }
        }
        try {
            return std::any_cast<T>(value);
        } catch(std::bad_any_cast const&) {
            std::string type;
            if(std::is_same<T, std::string>()) type = "string";
            if(std::is_same<T, float>()) type = "float";
            if(std::is_same<T, bool>()) type = "bool";
            throw std::runtime_error("Cannot convert data from location " + raw_key + " to type " + type);
        }
    }

private:
    void visitNode(AST* node, std::map<std::string, std::any> &map);
    std::string visitString(Value* node);
    float visitNumber(Value* node);
    bool visitBoolean(Value* node);
    std::vector<std::any> visitArray(Value* node);
    void generate();
};