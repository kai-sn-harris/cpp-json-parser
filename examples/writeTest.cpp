#include "../JSON.h"

int main() {
    JSON json("example json/write.json");
    std::cout << json.get<std::string>("varName") << std::endl;
    json.write("varName", "Goodbye");
    std::cout << json.get<std::string>("varName") << std::endl;
}