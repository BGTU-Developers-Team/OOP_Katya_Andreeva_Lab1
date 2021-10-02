#ifndef STOLYAROVA_LAB1_MYMATRIX_H
#define STOLYAROVA_LAB1_MYMATRIX_H

#include <iostream>
#include <malloc/malloc.h>
#include <string>
#include <stdexcept>
#include <sstream>
#include <vector>
#include "fort.hpp"

template<typename T>
class MyMatrix {
private:
    std::vector<int> linesLengths;
    int linesCounter = 0;
    std::vector<T> elements;

    int getBaseId(int currentLineId) {
        int idx = 0;
        for (int i = 0; i < currentLineId; ++i) {
            idx += linesLengths[i];
        }

        return idx;
    }


    void saveElement(int &elementsPerLineCounter, int &elementsCounter, std::string &stringBuilder) {
        T element; // переменная для временного хранения элемента матрицы (Для его сохранения)

        std::istringstream ss(
                stringBuilder); // мы берем нашу строку с записанными данными (новым элементом матрицы) и делаем из него поток данных для того, чтобы потом можно было сделать проверку, а можно ли его вообще записать в наше временную переменную.
        stringBuilder.clear(); // очищаем собранный из символов элемент матрицы
        if (ss
                >> element) { // Пытаемся записать строку, собранную до этого посимвольно, во временную переменную
            if (ss.str().find('.') != std::string::npos && element == std::stoi(ss.str())) {
                elements.clear();
                linesLengths.clear();

                // вызываем (выбрасываем) ошибку, где говорим, что значение некорректное и показываем, что за значение мы получили.
                throw std::invalid_argument("Parsing error! You are trying to parse double (" +
                                            ss.str() + ") to int.");
            }

            // Если успешно, то:
            elementsPerLineCounter++; // увеличиваем счетчик элементов в строке
            elementsCounter++; // увеличиваем счетчик всех элементов
            elements.push_back(element);
        } else {
            // Если не удалось записать собранную строчку во временную переменную, то:
            elements.clear();
            linesLengths.clear();

            throw std::invalid_argument("Error! Value is invalid: " + std::to_string(
                    element)); // вызываем (выбрасываем) ошибку, где говорим, что значение некорректное и показываем, что за значение мы получили.
        }
    }


public:

    // Коструктор пустной, следовательно поля у него пустые.
    MyMatrix() {
//        linesCounter = 0;
    }

    // Конструктор копирования. Нужно просто сохранить поля одного элемента в поля копии и готово.
    MyMatrix(const MyMatrix<T> &orig) {
        linesCounter = orig.linesCounter; // копируем кол-во строк
        linesLengths = orig.linesLengths;

        // Нужно выделить память для хранения всех элементов матрицы. А сколько надо памяти? Нужно узнать сколько нужно для хранения одного элемента и потом умножить это значение на кол-во элементов.
        int elementsCounter = 0; // Создадим счетчик элементов в матрице. Изначально 0
        for (int i = 0; i < linesCounter; ++i) {
            elementsCounter += linesLengths[i]; // Проходим по массиву строк
        }

        elements = orig.elements;
    }

    // Конструктор со стрингами (18+). Необходжимо преобразовать сроку и извлечь из нее все даннные.
    explicit MyMatrix(const std::string &matrixStr) {
        std::string errorProtocolMsg = "The stringified matrix must look like: {[1, 2, 3], [1.1, 1.2, 1.3]}"; // тут я просто сохранил сообщение ошибки, потому что буду использовать его много раз. Это хороший подход. Запомни его!

        if (matrixStr[0] == '{' && matrixStr.find('}') !=
                                   std::string::npos) { // Прежде всего нужно проверить какую строчку нам передали в конструктор: если в этой срочке нет, как минимум, символов { и }, то она уже неправильная и нет смысла её парсить(обрабатывать).
            if (matrixStr[1] ==
                '}') { // Если второй символ в строчке является }, то она пустая, а кто у нас пустую матрицу создает..? Правильно!
                MyMatrix<T>();
            } else {
                if (matrixStr[1] != '[') { // Если второй символ не [, то строка неверная, а значит выкидываем ошибку.
                    throw std::invalid_argument("Protocol error! The first '[' is missing. " + errorProtocolMsg);
                } else {
                    // Если все нормуль, то начием считывать первый ряд элементов:
                    bool isBracetOpened = true; // это переменная хранит кол-во открытых скобок в строке, потому что если эта переменная true и при этом встречается символ [, то нужно вызвать исключение, поскольку строка неверная.

                    int elementsCounter = 0; // счетчик элементов во всей матрице
                    int elementsPerLineCounter = 0; // счетчик элементов матрицы по строке

                    std::string stringBuilder;

                    for (int i = 2; matrixStr[i] != '}'; ++i) { // начинаем считывать элементы строки
                        if (matrixStr[i] == '[' &&
                            isBracetOpened) { // Постоянная проверка, что если скобка была до этого открывата и открывается снова, то это ошибка.
                            throw std::invalid_argument("Protocol error! At the " +
                                                        std::to_string(i + 1) +
                                                        "th place stays double '['" +
                                                        errorProtocolMsg);
                        }

                        if (matrixStr[i] == ']' &&
                            !isBracetOpened) { // Постоянная проверка, что если скобка была до этого закрыта и закрывается снова, то это ошибка.
                            throw std::invalid_argument("Protocol error! At the " +
                                                        std::to_string(i + 1) +
                                                        "th place stays double ']'" +
                                                        errorProtocolMsg);
                        }

                        switch (matrixStr[i]) {
                            case '[': { // Если линия открывается, то надо:
                                isBracetOpened = true; // Сменить флага

                                break;
                            }
                            case ']': { // Если линия закрывается, то надо:
                                isBracetOpened = false; // Сменить флаг
                                linesCounter++; // Увеличить счетчик строк

                                saveElement(elementsCounter, elementsPerLineCounter, stringBuilder);

                                // Сохранить длину свеже испеченной (добавленной) строки.
                                linesLengths.push_back(elementsPerLineCounter);
                                elementsPerLineCounter = 0; // обнулить счетчик элементов по строке

                                break;
                            }
                            case ',': { // Если запитая, то надо сохранить новый элемент матрицы
                                if (isBracetOpened) {
                                    saveElement(elementsCounter, elementsPerLineCounter, stringBuilder);
                                }

                                break;
                            }
                            case ' ': // Пустота она и в Африке пустота, а тут и подабно. Короче на пробел ничего делать не надо.
                                break;
                            default: { // Если у нас ничего из выше упомянутого не было, то запишем новый символ нового элемента матрицы
                                stringBuilder.push_back(matrixStr[i]);
                                break;
                            }
                        }
                    }
                }
            }
        } else {
            throw std::invalid_argument(
                    "Protocol error! Your string does not contain either '{' or '}'. " + errorProtocolMsg);
        }
    }

    // Конструктор из массива с допольнительными параметрами
    MyMatrix(const std::vector<T> &elements, const std::vector<int> &linesLengths, const int &linesCounter) {
        this->linesCounter = linesCounter;
        this->linesLengths = linesLengths;
        int elementsCounter = 0;
        for (int i = 0; i < linesCounter; ++i) {
            elementsCounter += linesLengths[i];
        }
        this->elements = elements;
    }

    // Деструктор. Его задача очистить память, где мы хранили _некогда_самые_дорогие_ данные, а сейчас они _пропадают_
    ~MyMatrix() {
        elements.clear();
        linesLengths.clear();
        linesCounter = 0;
    }

    void ConsoleWrite() {
        std::cout << std::endl;

        fort::char_table table;

        table.set_cell_text_align(fort::text_align::center);
        table.set_cell_min_width(7);


        if (linesCounter != 0) {
            int baseIdx;
            for (int i = 0; i < linesCounter; ++i) {
                baseIdx = getBaseId(i);
                for (int j = 0; j < linesLengths[i]; ++j) {
                    table << elements[baseIdx + j];
                }
                table << fort::endr;
            }
        } else {
            table.row(0).set_cell_content_text_style(fort::text_style::bold);
            table << fort::header << "Талица пустая";
        }


        std::cout << table.to_string() << std::endl;
    }
};

#endif //STOLYAROVA_LAB1_MYMATRIX_H
