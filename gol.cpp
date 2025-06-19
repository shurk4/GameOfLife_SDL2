#include "gol.h"

GOL::GOL()
{
    printCellsCoords(cells);
}

GOL::GOL(const int &_fieldX, const int &_fieldY) : GOL()
{
    fieldX = _fieldX;
    fieldY = _fieldY;
}

void GOL::addCell(const int &_x, const int &_y)
{
    if(getCellsNum() < 3)
    {
        cells[_x].insert(_y);
    }
}

void GOL::setCells(const std::map<int, std::set<int> > &_cells)
{
    cells = _cells;
}

void GOL::startCells()
{
    for(auto xCell = cells.begin(); xCell != cells.end(); xCell++)
    {
        for(auto yCell = xCell->second.begin(); yCell != xCell->second.end(); yCell++)
        {
            int x = xCell->first;
            int y = *yCell;
            uintptr_t neighborsCount = haveNeighbors(x, y);

            if(willLife(neighborsCount))
            {
                makeStep(x, y);
                continue;
            }
        }
    }

    cells = newCells;
    newCells.clear();
}

void GOL::makeStep(const int &_x, const int &_y)
{
    int x = getNewCoords(_x);
    int y = getNewCoords(_y);
    newCells[x].insert(y);
}

int GOL::getNewCoords(const int &_axisPoint)
{
    srand(static_cast<unsigned int>(std::time(nullptr)));
    int stepDir = std::rand() % 3;
    switch (stepDir) {
    case 0:
        return _axisPoint - 1;
        break;
    case 1:
        return _axisPoint;
        break;
    case 2:
        return _axisPoint + 1;
        break;
    }
    return _axisPoint;
}

void GOL::getRange(int &_x, int &_y, int &_xMax, int &_yMax)
{
    int x = _x - 1;
    int y = _y - 1;

    if(x <= 0) _x = 0;
    else _x = x;

    if(y <= 0) _y = 0;
    else _y = y;

    _xMax = x + 3;
    _yMax = y + 3;

    if(_xMax >= fieldX) _xMax = fieldX;
    if(_yMax >= fieldY) _yMax = fieldY;
}

int GOL::haveNeighbors(const int &_x, const int &_y, bool _newCells)
{
    int x = _x;
    int y = _y;
    int xMin = _x;
    int yMin = _y;
    int xMax;
    int yMax;
    getRange(xMin, yMin, xMax, yMax);

    int neighborsCount = 0;

    bool isCurrentCellLife = isLifeCell(_x, _y);

    for(int xCell = xMin; xCell < xMax; xCell++)
    {
        for(int yCell = yMin; yCell < yMax; yCell++)
        {
            if(!_newCells)
            {
                if(isEmptyCell(xCell, yCell) && haveNeighbors(xCell, yCell, true) == 3)
                {
                    newCells[xCell].insert(yCell);
                }
            }

            if(isLifeCell(xCell, yCell) && (xCell != _x || yCell != _y))
            {
                neighborsCount++;
            }
        }
    }

    return neighborsCount;
}

bool GOL::isEmptyCell(const int &_x, const int &_y)
{
    return cells.find(_x) == cells.end() || !cells.at(_x).count(_y);
}

bool GOL::isLifeCell(const int &_x, const int &_y)
{
    return cells.find(_x) != cells.end() && cells.at(_x).count(_y);
}

bool GOL::willLife(const int &_neighborsCount)
{
    return _neighborsCount >= 2 && _neighborsCount <= 3;
}

bool GOL::willBorn(const int &_neighborsCount)
{
    return _neighborsCount == 3;
}

int GOL::getCellsNum()
{
    int result = 0;
    for(auto x = cells.begin(); x != cells.end(); x++)
    {
        for(auto y = x->second.begin(); y != x->second.end(); y++)
        {
            result ++;
        }
    }
    return result;
}

std::map<int, std::set<int> > GOL::getCells()
{
    return cells;
}

void GOL::printCellsCoords(const std::map<int, std::set<int> > &_cells)
{
    for(auto xCell = _cells.begin(); xCell != _cells.end(); xCell++)
    {
        for(auto yCell = xCell->second.begin(); yCell != xCell->second.end(); yCell++)
        {
            int x = xCell->first;
            int y = *yCell;
            std::cout << "x = " << x << " y = " << y << std::endl;
        }
    }
}

void GOL::addSpaces(int _num)
{
    std::cout << _num << " ";
    for(int i = 0; i < _num; i++)
    {
        std::cout << "   ";
    }
}
