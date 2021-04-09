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
    std::string fileName;
    JSON(std::string fileName);

    template <class T>
    T get(std::string key) {
        std::string raw_key = key;
        bool isRootArray = false;
        if(key.substr(0, 4) == "(ar)") {
            isRootArray = true;
            key.erase(0, 4);
        }
        std::vector<std::string> keys = this->genKeyList(key);

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
            std::string type = "unknown or invalid type";
            if(std::is_same<T, std::string>()) type = "string";
            if(std::is_same<T, float>()) type = "float";
            if(std::is_same<T, bool>()) type = "bool";
            throw std::runtime_error("Cannot convert data from location " + raw_key + " to type " + type);
        }
    }

    // Modifiying an existing json file
    /*
        This function modifies the AST and then will modify
        a json file out of the values stored in that AST. 
        A flag is available to determine whether or not the
        user would like the json file to be modified (true by default)
    */
    template <class T>
    void write(std::string key, T value, bool editFile=true) {
        /* 
            convert key into a vector of key segments.
            Given the key "(ar)users.0.name" the array of segments is: ["users", "0", "name"]
        */
        std::vector<std::string> segs;
        segs = this->genKeyList(key);

        /*
            Modify the node at the key's location in the AST
            then rewrite the file's json using the modified AST`
        */

        // Gen the AST so that we can modify it
        auto ast = Parser(Lexer(this->text)).ast();
        this->findAndModAST(ast, segs, value);
        // Rewrite the json file
        this->text = "";
        this->rewriteJSON(ast);
        if(editFile) {
            // write newly generated text to json file
            std::ofstream file;
            file.open(this->fileName, std::ofstream::out | std::ofstream::trunc);
            file << this->text;
            file.close();
        }

        // refresh the AST so that this->get<>() works
        // empty variables and root array
        std::map<std::string, std::any> map;
        this->variables = map;
        std::vector<std::any> rootArray;
        this->rootArray = rootArray;

        Lexer lexer(this->text);
        Parser parser(lexer);
        this->visitNode(parser.ast(), this->variables);
    }

private:
    void generate();
    // reading json
    void visitNode(std::shared_ptr<AST> node, std::map<std::string, std::any> &map);
    std::string visitString(std::shared_ptr<Value> node);
    float visitNumber(std::shared_ptr<Value> node);
    bool visitBoolean(std::shared_ptr<Value> node);
    std::vector<std::any> visitArray(std::shared_ptr<Value> node);
    std::vector<std::string> genKeyList(std::string key) {
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
        return keys;
    }
    
    /*
        Function takes in an AST, key segments, and a value.
        It will then find the node the key (in segmented form)
        points to and modify it's value
    */
    template <class T>
    void findAndModAST(std::shared_ptr<AST> &ast, std::vector<std::string> segs, T value) {
        if(ast->ASTtype == "value" && std::dynamic_pointer_cast<Value>(ast)->type == "object") {
            for(auto elem : std::dynamic_pointer_cast<Object>(ast)->values) {
                if(elem->left->value == segs[0]) {
                    if(segs.size() > 1) segs.erase(segs.begin());
                    this->findAndModAST((std::shared_ptr<AST>&)elem, segs, value);
                    break;
                }
            }
        } else if(ast->ASTtype == "value" && (std::dynamic_pointer_cast<Value>(ast)->type != "object" && std::dynamic_pointer_cast<Value>(ast)->type != "array")) {
            ast = this->createNode(value);
        } else if(ast->ASTtype == "value" && std::dynamic_pointer_cast<Value>(ast)->type == "array") {
            if(std::stoi(segs[0]) < std::dynamic_pointer_cast<Array>(ast)->values.size()) {
                if(segs.size() > 1) segs.erase(segs.begin());
                this->findAndModAST((std::shared_ptr<AST>&)std::dynamic_pointer_cast<Array>(ast)->values[std::stoi(segs[0])], segs, value);
            }
        } else if(ast->ASTtype == "assign") {
            auto node = std::dynamic_pointer_cast<Assign>(ast);
            if(node->right->type == "object" || node->right->type == "array") this->findAndModAST((std::shared_ptr<AST>&)node->right, segs, value);
            else if(node->left->value == segs[0]) {
                /*
                    Will add JSON object classes that are user friendly in coming updates so that it is possible
                    to modify a whole json object/array instead of an individual value
                */
                node->right = this->createNode(value);
            }
        }
    }

    std::shared_ptr<Value> createNode(std::string value);
    std::shared_ptr<Value> createNode(const char* value);
    std::shared_ptr<Value> createNode(bool value);
    std::shared_ptr<Value> createNode(float value);
    std::shared_ptr<Value> createNode(long long int value);

    void rewriteJSON(std::shared_ptr<AST> ast);
    void writeVal(std::string type, std::shared_ptr<Value> value);
};