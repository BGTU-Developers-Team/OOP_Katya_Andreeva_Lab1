//Вариант 8
//Матрица переменной размерности, представленная динамическим массивом, в котором строки матрицы расположены последовательно друг за другом.

#include <iostream>
#include "MyMatrix.h"

int main() {
    auto myMatrix = new MyMatrix<int>("{[1,2,3],[4,5,6]}");

    myMatrix->ConsoleWrite();

    return 0;
}
