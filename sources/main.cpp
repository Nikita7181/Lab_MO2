#include <iostream>
#include "simplex.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        throw std::runtime_error ("Path is required as parameter!");
    }
    std::string jsonPath = argv[1];

    std::ifstream file{jsonPath};
    if (!file) {
        throw std::runtime_error{"unable to open json: " + jsonPath};
    }

    json data;

    try {
        file >> data;
    }
    catch (std::exception & e)
    {
        throw std::runtime_error{"Wrong json format"};
    }


    std::vector<double > c;
    std::vector<double > b;
    std::vector<std::vector<double>> a;

    if (data.at("C").is_array())
    {
        c = data.at("C").get<std::vector<double>>();
        b = data.at("B").get<std::vector<double>>();
        a = data.at("A").get<std::vector<std::vector<double>>>();
    }

    Simplex s (a, b, c);

    s.calculate();

    return 0;
}
