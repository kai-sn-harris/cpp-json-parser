#include <iostream>
#include "../JSON.h"

int main(int argc, char* argv[]) {
    JSON json("../example json/b.json");
    json.generate();
    std::cout << json.get<std::string>("web-app.servlet.0.servlet-name") << std::endl;
}

// g++ -std=c++17 b.cpp ../JSON.cpp ../lexer.cpp ../parser.cpp -o b