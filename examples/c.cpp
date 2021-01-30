#include "../JSON.h"
#include <iostream>

int main() {
    JSON json("../example json/c.json");
    // json.generate();
    std::cout << json.get<std::string>("(ar)0.0.objectInArrayInRootArray") << std::endl;
    std::cout << json.get<float>("(ar)1.object in root array") << std::endl;
}