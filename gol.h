#ifndef GOL_H
#define GOL_H

#include <iostream>
#include <map>
#include <set>
#include <chrono>

class GOL
{
public:
    GOL();
    GOL(const int &_fieldX, const int &_fieldY);

    void addCell(const int &_x, const int &_y);
    void setCells(const std::map<int, std::set<int>> &_cells);

    // Запускает шаг жизни клеток
    void startCells();

    // Создаёт новую клетку в карте клеток нового хода
    // void newCell(const int& _x, const int& _y);

    // Делает шаг клетки если клетка продолжает жить
    void makeStep(const int& _x, const int& _y);

    // Получение новых координат
    int getNewCoords(const int& _axisPoint);

    // Записывает начальные и конечные координаты поиска соседей в полученные переменные
    void getRange(int &_x, int &_y, int &_xMax, int &_yMax);

    // Возвращает количество имеющихся соседей
    int haveNeighbors (const int& _x, const int& _y, bool _newCells = false);

    // Проверка координат на наличие клетки
    bool isEmptyCell(const int& _x, const int& _y);
    bool isLifeCell(const int& _x, const int& _y);

    // Принимает количество соседей и вовращает статус дальнейшего действия с клеткой
    bool willLife(const int& _neighborsCount);
    bool willBorn(const int& _neighborsCount);

    // Возвращает количество клеток на поле
    int getCellsNum();

    // Получить списки клеток
    std::map<int, std::set<int>> getCells();
    // std::map<int, std::set<int>> getNewCells();

    // Дебаг
    void printCellsCoords(const std::map<int, std::set<int>> &_cells);
    void addSpaces(int _num);

private:
    // Клетки текущего хода
    std::map<int, std::set<int>> cells;
    // Клетки нового хода
    std::map<int, std::set<int>> newCells;

    // Размеры поля
    int fieldX = 10;
    int fieldY = 10;

    int maxCells = 3;
};

#endif // GOL_H
