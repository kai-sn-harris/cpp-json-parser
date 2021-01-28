#include <iostream>
#include "JSON.h"

int main(int argc, char* argv[]) {
    JSON json("example json/a.json");
    json.generate();
    auto array = std::any_cast<std::vector<std::any>>(json.variables["array"]);
    auto string = std::any_cast<std::string>(array[0]);
    auto objInArray = std::any_cast<std::map<std::string, std::any>>(array[1]);
    auto stringInObjInArray = std::any_cast<std::string>(objInArray["objInArray"]);
    std::cout << string << std::endl;
    std::cout << stringInObjInArray << std::endl;
}