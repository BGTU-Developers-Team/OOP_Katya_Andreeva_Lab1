//Вариант 8
//Матрица переменной размерности, представленная динамическим массивом, в котором строки матрицы расположены последовательно друг за другом.

#include <iostream>
#include "MyMatrix.h"

int main() {
    //TODO make construct work with float
//    auto myMatrix = new MyMatrix<float>("{[1.5F, 2F, 13F, 3F], [1F,2F,3F] ,[4F,5F,6F]}");
    auto myMatrix = new MyMatrix<double>("{[1.23, 2.28, 13, 3.14], [1,2,3] ,[4,5,6]}");
//    auto mm1 = new MyMatrix<int>(*myMatrix);
    auto mm2 = new MyMatrix<double>("{}");

    myMatrix->ConsoleWrite();
//    mm1->ConsoleWrite();
    mm2->ConsoleWrite();
    return 0;
}
