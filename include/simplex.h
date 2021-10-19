#pragma once
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
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
    bool dual;

    bool canSolve;
    bool infinite;



public:
    Simplex();
    Simplex(std::vector <std::vector<double>> a, std::vector<double> b, std::vector<double> c);
    bool optimal();
    int findRow();
    int findColumn();
    void calculate();
    void doTransform(int row, int col);
    void endless (int column);
    void getElem( int & row, int & col);
    void print();
    void print2();
    void makeDual();
};