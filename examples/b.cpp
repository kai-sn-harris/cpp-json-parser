#include <iostream>
#include "../JSON.h"

int main(int argc, char* argv[]) {
    JSON json("../example json/b.json");
    json.generate();
    
    auto servlet0 = json.get<std::vector<std::any>>("web-app.servlet", json.variables)[0];
    auto servlet0Obj = json.anyToObj(servlet0);
    std::string servlet0Name = json.get<std::string>("servlet-name", servlet0Obj);
    std::cout << servlet0Name << std::endl;
}

// g++ -std=c++17 b.cpp ../JSON.cpp ../lexer.cpp ../parser.cpp -o b