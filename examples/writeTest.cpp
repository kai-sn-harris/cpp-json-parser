#include "../JSON.h"

int main() {
    JSON json("example json/write.json");
    std::cout << "obj.asd : " << json.get<std::string>("obj.asd") << std::endl;
    json.write("obj.asd", 5.0F);
    std::cout << "obj.asd : " << json.get<float>("obj.asd") << std::endl;
    json.write("obj.asd", true);
    std::cout << "obj.asd : " << (json.get<bool>("obj.asd") ? "true" : "false") << std::endl;
    json.write("obj.asd", NULL);
    std::cout << "obj.asd : " << json.get<std::string>("obj.asd") << std::endl;
    
    std::cout << "array.0 : " << json.get<std::string>("array.0") << std::endl;
    json.write("array.0", 4.56F, false);
    std::cout << "array.0 : " << json.get<float>("array.0") << std::endl;
}