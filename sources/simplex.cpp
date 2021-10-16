#include "simplex.h"

template <typename T>
std::string toString (const T val)
{
    std::stringstream ss;
    ss << val;
    return ss.str();
}

void swap(std::string &a, std::string &b)
{
    std::string c = b;
    b = a;
    a = c;
}

Simplex::Simplex()
{}

Simplex::Simplex(std::vector<std::vector<double>> a, std::vector<double> b, std::vector<double> c)
{
    sizey = a.size();
    sizea = a[0].size();
    sizex = sizea + sizey;
    sizec = c.size() + sizey;
    funcMax = 0;
    canSolve = true;
    infinite = false;
    A.resize(sizey, std::vector<double>(sizex));
    B.resize(b.size());
    C.resize(sizec);

    for (int i = 0; i < sizey; i++)
        for(int j = 0; j < sizea; j++)
            A[i][j] = a[i][j];
    for (int i = 0; i < sizey; i++)
        for(int j = sizea; j < sizex; j++)
        {
            if (i == j - sizea)
                A[i][j] = 1;
            else
                A[i][j] = 0.0;
        }

    for (int i = 0; i < b.size(); i++)
        B[i] =b[i];
    for (int i = 0; i < c.size(); i++)
    {
        //C[i] = -1 * c[i];
        C[i] = c[i];
    }

    titleCol.resize(sizey);
    titleRow.resize(sizex);

    for (int i = 0; i < sizey; i++)
        titleCol[i] = 'x' + toString(i + sizea + 1);
    for (int i = 0; i < sizex; i++)
        titleRow[i]='x'+ toString(i+1);

}

void Simplex::print()
{
    std::cout << std::fixed << std::setprecision(3) << "B = {";
    for (int i = 0 ; i < B.size(); i++)
    {
        std::cout << B[i];
        if (i != B.size()-1)
            std::cout << ", ";
    }
    std::cout << "}\n";

    std::cout << "C = {";
    for (int i = 0 ; i < C.size(); i++)
    {
        std::cout << C[i];
        if (i != C.size()-1)
            std::cout << ", ";
    }
    std::cout << "}\n";

    std::cout << "A = {\n";
    for (int i = 0 ; i < sizey; i++)
    {
        std::cout << " {";
        for (int j = 0; j < sizex; j++)
        {
            std::cout << A[i][j];
            if (j != sizex-1)
                std::cout << ", ";
        }
        std::cout << "}";
        if (i != sizey-1)
        {
            std::cout << ",\n";
        }

    }
    std::cout << "\n}\n***************************************************************************\n";


}

bool Simplex::optimal()
{
    for (int i = 0; i < C.size(); i++)
        if (C[i] > 0)
        {
            return false;
        }

    return true;
}

int Simplex::findColumn() {
    double max = 0;
    int index = 0;

    for (int i = 0; i < C.size(); i++)
        if (C[i] > max)
        {
            max = C[i];
            index = i;
        }

    return index;
}

void Simplex::endless (int column)
{
    int Count = 0;
    for (int i = 0 ; i < B.size(); i++)
    {
        if (A[i][column] < 0)
            Count++;
    }
    if (Count == B.size())
    {
        infinite = true;
    }
}


int Simplex::findRow() {

    int index = 0;
    int col = findColumn();
    double min = B[index] / A[index][col];

    int Count = 0;
    for (int j = 0; j < B.size(); j++)
    {
        for (int i = 0; i < B.size (); i++)
        {
            if (A[j][i] >= 0 && B[j] < 0)
                Count++;
        }
    }
    if (Count == B.size())
    {
        canSolve = false;
    }

    for (int i = 1; i < B.size(); i++)
    {
        if (B[i] / A[i][col] < min)
        {
            min = B[i] / A[i][col];
            index = i;
        }
    }

    return index;
}

void Simplex::doTransform(int row, int col)
{
    double elem = A[row][col];

    std::vector<double> oldRow; oldRow.resize(sizex);
    std::vector<double> oldCol; oldCol.resize(sizey);
    std::vector<double> newRow; newRow.resize(sizex);
    std::vector<double> newCol; newCol.resize(sizey);

    funcMax = funcMax - (C[col]*(B[row]/elem))    ;

    for (int i = 0; i < sizex; i++)
        oldRow[i]=A[row][i];

    for (int i = 0; i < sizey; i++)
        oldCol[i]=A[i][col];

    for (int i = 0; i < sizex; i++)
        newRow[i] = oldRow[i]/elem;

    for (int i = 0; i < sizey; i++)
        newCol[i] = -1 * oldCol[i]/elem;

    for (int i = 0; i < sizey; i++)
        for(int j = 0; j < sizex; j++)
            A[i][j] = A[i][j] - oldCol[i]/elem * oldRow[j] + 0.0;

    for (int i = 0; i < sizey; i++)
        A[i][col] = newCol[i] + 0.0;

    for (int i = 0; i < sizex; i++)
        A[row][i] = newRow[i] + 0.0;

    B[row]=B[row]/elem + 0.0;
    for (int i = 0; i < sizey; i++)
        if (i!=row)
            B[i]= B[i] - oldCol[i]* B[row] + 0.0;

    for (int i = 0; i < sizex; i++)
        if (i!=col)
            C[i]=C[i]-C[col]/elem*oldRow[i] + 0.0;

    C[col] = -1 * C[col]/elem + 0.0;

    swap(titleCol[col], titleRow[row]);
}

void Simplex::calculate()
{
    int step = 0;
    while ( !optimal() && canSolve)
    {
        endless(findColumn());
        if (infinite)
            break;
        step++;
        print2();
        doTransform(findRow(), findColumn());

    }

    if (canSolve && !infinite)
    {
        std::cout << "Function maximum value: " << -funcMax << "\n";
        print2();

        for (int i = 0; i < sizey; i++) {
            for (int j = 0; j < sizey; j++) {

                if (titleCol[i] == 'x' + toString(j + 1)) {
                    std::cout << " X" << toString(i + 1) << " = " << B[i] << " ";
                    break;
                } else if (j == sizey - 1) {
                    std::cout << " X" << toString(j) << " = 0";

                }
            }
        }
    }
    else if (infinite)
    {
        std::cerr << "Function has infinite number of solutions" <<  "\n";
    }
    else
    {
        std::cerr << "Can't solve...";
    }
}

void Simplex::print2()
{
    std::cout << std::fixed << std::setprecision(3) << " \t  S0\t";
    for (int i = 0; i < sizex-sizea; i++) {
        std::cout << "   " << titleRow[i] << "\t";

    }
    std::cout << "\n";
    for (int i = 0; i < sizey; i++) {
        std::cout << titleCol [i] << "\t";
        B[i] < 0 ?  std::cout  << B[i]: std::cout << " " << B[i];
        std::cout << "\t";
        for (int j = 0; j < sizex-sizea; j++)
        {
            A[i][j] < 0 ? std::cout << A[i][j] << "\t" : std::cout << " " << A[i][j] << "\t" ;
        }
        std::cout << "\n";
    }
    std::cout <<"F\t"; funcMax < 0 ?  std::cout  << funcMax : std::cout << " " << funcMax;
    std::cout << "\t";
    for (int i = 0; i < sizex-sizea; i++)
    {
        C[i] < 0 ? std::cout << C[i] << "\t" : std::cout << " " << C[i] << "\t" ;
    }
    std::cout << "\n\n***************************************************************************\n";

}
