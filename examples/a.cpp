#include <iostream>
#include "../JSON.h"

int main(int argc, char* argv[]) {
    JSON json("../example json/a.json");
    json.generate();
    std::cout << json.get<std::string>("string") << std::endl;
    std::cout << json.get<std::string>("obj.stringInObj") << std::endl;
    std::cout << json.get<float>("number") << std::endl;
    auto array = json.get<std::vector<std::any>>("array");
    std::cout << std::any_cast<std::string>(array[1]);
}