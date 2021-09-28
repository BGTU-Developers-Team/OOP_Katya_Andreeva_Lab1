#ifndef STOLYAROVA_LAB1_MYMATRIX_H
#define STOLYAROVA_LAB1_MYMATRIX_H

#include <iostream>
#include <malloc/malloc.h>
#include <string>
#include <stdexcept>
#include <sstream>

template<typename T>
class MyMatrix {
private:
    int *linesLengths;

    int linesCounter = 0;

    T *elements;

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
                >> element) { // Пытаемся записать строку, собранную до того посимвольно, во временную переменную
            // Если успешно, то:
            elementsPerLineCounter++; // увеличиваем счетчик элементов в строке
            elementsCounter++; // увеличиваем счетчик всех элементов
            elements = static_cast<T *>(realloc(elements, sizeof(T) *
                                                          elementsCounter)); // выделяем дополнительную память для нового элемента
            elements[elementsCounter - 1] = element; // сохраняем в новую память этот элемент
        } else {
            // Если не удалось записать собранную строчку во временную переменную, то:
            delete[] elements; // очистить память, где хранились элементы. Они уже не нужны, потому что матрица неправильно пришла на обработку.
            elements = nullptr; // наведем на НИЧЕГО (NULL или же nullptr)

            delete[] linesLengths; // очистить память, где хранились длины строок. Они уже не нужны, потому что матрица неправильно пришла на обработку.
            linesLengths = nullptr; // наведем на НИЧЕГО (NULL или же nullptr)

            linesCounter = 0; // обнуолим счетчик строк

            throw std::invalid_argument("Error! Value is invalid: " + std::to_string(
                    element)); // вызываем (выбрасываем) ошибку, где говорим, что значение некорректное и показываем, что за значение мы получили.
        }
    }


public:

    // Коструктор пустной, следовательно поля у него пустые.
    MyMatrix() {
        linesLengths = nullptr;
        elements = nullptr;
    }

    // Конструктор копирования. Нужно просто сохранить поля одного элемента в поля копии и готово.
    MyMatrix(const MyMatrix<T> &orig) {
        linesCounter = orig.linesCounter; // копируем кол-во строк
        linesLengths = static_cast<int *>(malloc(
                sizeof(int) * linesCounter)); // выделяем память для хранения длин строк.
        std::memcpy(linesLengths, orig.linesLengths, sizeof(int) *
                                                     linesCounter); // не забываем скопировать длины строк из оригинало. А ни то память заняли, а она всякий мусор хранит... НЕХОРОШО!

        // Нужно выделить память для хранения всех элементов матрицы. А сколько надо памяти? Нужно узнать сколько нужно для хранения одного элемента и потом умножить это значение на кол-во элементов.
        int elementsCounter = 0; // Создадим счетчик элементов в матрице. Изначально 0
        for (int i = 0; i < linesCounter; ++i) {
            elementsCounter += linesLengths[i]; // Проходим по массиву строк
        }
        elements = static_cast<T>(malloc(sizeof(T) * elementsCounter)); // выделяем память для элементов матрицы
        std::memcpy(elements, orig.elements, sizeof(T) *
                                             elementsCounter); // ты уже хотел забыть скопировать элементы матрицы?! Ничего, я напомню ;)
    }

    // Конструктор со стрингами (18+). Необходжимо преобразовать сроку и извлечь из нее все даннные.
    MyMatrix(const std::string &matrixStr) {
        elements = static_cast<T *>(malloc(sizeof(T)));
        linesLengths = static_cast<int *>(malloc(sizeof(int)));

        auto errorProtocolMsg = "The stringified matrix must look like: {[1,2,3], [1.1, 1.2, 1.3]}"; // тут я просто сохранил сообщение ошибки, потому что буду использовать его много раз. Это хороший подход. Запомни его!

        if (matrixStr[0] == '{' && matrixStr.find('}') !=
                                   std::string::npos) { // Прежде всего нужно проверить какую строчку нам передали в конструктор: если в этой срочке нет, как минимум, символов { и }, то она уже неправильная и нет смысла её парсить(обрабатывать).
            if (matrixStr[1] ==
                '}') { // Если второй символ в строчке является }, то она пустая, а кто у нас пустую матрицу создает..? Правильно!
                MyMatrix<T>();
            } else {
                if (matrixStr[1] != '[') { // Если второй символ не [, то строка неверная, а значит выкидываем ошибку.
                    throw std::invalid_argument(errorProtocolMsg);
                } else {
                    // Если все нормуль, то начием считывать первый ряд элементов:
                    bool isBracetOpened = true; // это переменная хранит кол-во открытых скобок в строке, потому что если эта переменная true и при этом встречается символ [, то нужно вызвать исключение, поскольку строка неверная.

                    int elementsCounter = 0; // счетчик элементов во всей матрице
                    int elementsPerLineCounter = 0; // счетчик элементов матрицы по строке

                    std::string stringBuilder;

                    for (int i = 2; matrixStr[i] != '}'; ++i) { // начинаем считывать элементы строки
                        if (matrixStr[i] == '[' &&
                            isBracetOpened) { // Постоянная проверка, что если скобка была до этого открывата и открывается снова, то это ошибка.
                            throw std::invalid_argument(errorProtocolMsg);
                        }

                        if (matrixStr[i] == ']' &&
                            !isBracetOpened) { // Постоянная проверка, что если скобка была до этого закрыта и закрывается снова, то это ошибка.
                            throw std::invalid_argument(errorProtocolMsg);
                        }

                        switch (matrixStr[i]) {
                            case '[': { // Если линия закрывается, то надо:
                                isBracetOpened = true; // Сменить флага
                                linesCounter++; // Увеличить счетчик строк
                                linesLengths = static_cast<int *>(realloc(linesLengths, sizeof(int) *
                                                                                        linesCounter)); // Увеличить памтяь под хранение строк

                                break;
                            }
                            case ']': { // Если линия закрывается, то надо:
                                isBracetOpened = false; // Сменить флаг
                                linesCounter++; // увеличиваем счетчик линий

                                saveElement(elementsCounter, elementsPerLineCounter, stringBuilder);

                                linesLengths[linesCounter -
                                             1] = elementsPerLineCounter; // Сохранить длину свеже испеченной (добавленной) строки.
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
            throw std::invalid_argument(errorProtocolMsg);
        }
    }

    // Деструктор. Его задача очистить память, где мы хранили _некогда_самые_дорогие_ данные, а сейчас они _пропадают_
    ~MyMatrix() {
        delete[] elements;
        delete[] linesLengths;
        linesCounter = 0;
    }

    void ConsoleWrite() {
        std::cout << std::endl;

        int baseIdx;
        for (int i = 0; i < linesCounter; ++i) {
            baseIdx = getBaseId(i);
            for (int j = 0; j < linesLengths[i]; ++j) {
                std::cout << elements[baseIdx] << '\t';
            }
            std::cout << std::endl;
        }
    }

};

#endif //STOLYAROVA_LAB1_MYMATRIX_H
