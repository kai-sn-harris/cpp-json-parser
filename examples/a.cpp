#include <iostream>
#include "../JSON.h"

int main(int argc, char* argv[]) {
    JSON json("../example json/a.json");
    json.generate();
    std::cout << json.get<std::string>("string", json.variables) << std::endl;
    std::cout << json.get<std::string>("obj.stringInObj", json.variables) << std::endl;
    std::cout << json.get<float>("number", json.variables) << std::endl;
    auto array = json.get<std::vector<std::any>>("array", json.variables);
    std::cout << std::any_cast<std::string>(array[1]);
}