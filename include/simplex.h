#pragma once
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"

class Simplex
{
    int sizex, sizey, sizec, sizea;
    std::vector <std::vector<double>> A;
    std::vector<double> B;
    std::vector<double> C;
    double funcMax;
    std::vector<std::string> titleRow;
    std::vector<std::string> titleCol;
    bool canSolve;
    bool infinite;

    Simplex();
    bool optimal();
    int findRow();
    int findColumn();
    void doTransform(int row, int col);
    void endless (int column);

public:
    Simplex(std::vector <std::vector<double>> a, std::vector<double> b, std::vector<double> c);
    void calculate();
    void print();
    void print2();
};
