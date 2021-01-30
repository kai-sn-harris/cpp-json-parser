#include <iostream>
#include "../JSON.h"

int main(int argc, char* argv[]) {
    JSON json("../example json/a.json");
    json.generate();
    std::cout << json.get<std::string>("string", json.variables) << std::endl;
    std::cout << json.get<std::string>("obj.stringInObj", json.variables) << std::endl;
    std::cout << json.get<float>("number", json.variables) << std::endl;
    std::cout << json.get<float>("array.0.how", json.variables) << std::endl;
    std::cout << json.get<std::string>("array.1", json.variables) << std::endl;
}