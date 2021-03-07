#include "../JSON.h"

int main() {
    JSON json("example json/write.json");
    std::cout << json.get<std::string>("obj.asd") << std::endl;
    json.write("obj.asd", "goodbye");
    std::cout << json.get<std::string>("obj.asd") << std::endl;
}