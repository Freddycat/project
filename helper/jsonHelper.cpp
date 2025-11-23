
#include <fstream>
#include <filesystem>
#include <iostream>

#include "jsonHelper.h"
#include "global.h"

using json = nlohmann::json;
using std::filesystem::path;


json parseFile()
{
    path map = g.home / "maps" / "map.json";
    std::cout << "path" << map << std::endl;
    std::ifstream f(map);
    json data = json::parse(f);
    return data;
}