#include <iostream>
#include "simplex.h"
#include "nlohmann/json.hpp"
#include <fstream>



using json = nlohmann::json;
int main(int argc, char * argv[])
{
    //std::string fileName = argv[1];
    //Simplex s({{2,1,1},{1,1,0},{0,0.5,2}}, {3,6,3}, {5,3,8});
    //Simplex s({{2,4},{1,1},{2,1}}, {560,170,300}, {4,5});
    //Simplex s({{4,1},{-1,1}}, {8,3}, {3,4});


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
    catch (std::exception e)
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
