#include <iostream>
#include "JSON.h"

int main(int argc, char* argv[]) {
    JSON json("example json/a.json");
    json.generate();
    std::cout << std::any_cast<std::string>(json.variables["string"]) << std::endl;
    std::cout << std::any_cast<float>(json.variables["num"]) << std::endl;
}