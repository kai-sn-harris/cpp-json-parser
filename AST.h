#pragma once
#include <string>
#include <vector>

class AST {
public:
    std::string ASTtype;
    AST(std::string ASTtype): ASTtype(ASTtype) {}
    virtual ~AST() = default;
};

class Value : public AST {
public:
    std::string type;
    Value(std::string type): type(type), AST("value") {}
    virtual ~Value() = default;
};

class String : public Value {
public:
    std::string value;
    String(std::string value): value(value), Value("string") {}
};

class Number : public Value {
public:
    float value;
    Number(float value): value(value), Value("number") {}
};

class Boolean : public Value {
public:
    bool value;
    Boolean(bool value): value(value), Value("boolean") {}
};

class Null : public Value {
public:
    Null(): Value("null") {}
};

class Assign : public AST {
public:
    String* left;
    Value* right;
    Assign(String* left, Value* right):
        left(left), right(right), AST("assign") {}
};

class Object : public Value {
public:
    std::vector<Assign*> values;
    Object(std::vector<Assign*> values): values(values), Value("object") {}
    Object(): Value("object") {}
};

class Array : public Value {
public:
    std::vector<Value*> values;
    Array(std::vector<Value*> values): values(values), Value("array") {}
};