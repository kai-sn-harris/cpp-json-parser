#include <iostream>
#include "../JSON.h"

int main(int argc, char* argv[]) {
    JSON json("../example json/a.json");
    json.generate();
    std::cout << json.get<std::string>("string") << std::endl;
    std::cout << json.get<std::string>("obj.stringInObj") << std::endl;
    std::cout << json.get<float>("number") << std::endl;
    std::cout << json.get<float>("array.0.how") << std::endl;
    std::cout << json.get<std::string>("array.2.0") << std::endl;
    std::cout << json.get<std::string>("array.2.1.0") << std::endl;
}