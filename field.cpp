#include "field.h"

void Field::clearCellsVec(std::vector<Cell *> &_cells)
{
    for(auto p : _cells)
    {
        delete p;
    }
}

Field::Field(std::shared_ptr<SDL_Renderer>&_renderer)
{
    renderer = _renderer;
    fillCellsCoords();
}

bool Field::isFieldInFocus(const int &_mouseX, const int &_mouseY)
{
    return _mouseX >= fieldXPos_start &&
           _mouseX <= fieldXPos_end &&
           _mouseY >= fieldYPos_start &&
           _mouseY <= fieldYPos_end;
}

std::pair<int, int> Field::getCellNum(const int &_mouseX, const int &_mouseY)
{
    std::cout << "Field::getCellNum" << std::endl;
    int x = -1;
    int y = -1;
    if(isFieldInFocus(_mouseX, _mouseY))
    {
        x = (_mouseX - fieldXPos_start) / (cellXSize + spacerSize);
        y = (_mouseY - fieldYPos_start) / (cellYSize + spacerSize);
    }
    std::cout << "Field::getCellNum x: " << x << " y: " << y << std::endl;
    return std::pair(x, y);
}

std::pair<int, int> Field::getCellCoords(const int &_x, const int &_y)
{
    int x = -1;
    int y = -1;

    if(_x > -1 && _x < fieldXCells &&
        _y > -1 && _y < fieldYCells)
    {
        x = cellsCoords[_x][_y].first;
        y = cellsCoords[_x][_y].second;
    }

    return std::pair(x, y);
}

void Field::showCells(const std::map<int, std::set<int> > &_cells)
{
    // Позиция текущей клетки
    int x;
    int y;

    if(_cells.size() > 0)
    {
        // std::cout << "Field::showCells" << std::endl;

        // Проверка изменившихся клеток
        for(auto xCell = _cells.begin(); xCell != _cells.end(); xCell++)
        {
            if(xCell->second.empty()) continue;
            for(auto yCell = xCell->second.begin(); yCell != xCell->second.end(); yCell++)
            {
                x = xCell->first;
                y = *yCell;

                if(x < 0 || x > fieldXCells || y < 0 || y > fieldYCells)
                {
                    std::cout << "---   Cell position number wrong x: " << x << " y: " << y << std::endl;
                    continue;
                }

                if(cellsMap.find(x) != cellsMap.end() && cellsMap.at(x).count(y))
                {
                }
                else
                {
                    std::pair<int, int> cellCoords = getCellCoords(x, y);

                    if(cellCoords.first < fieldXPos_start || cellCoords.first > fieldXPos_end || cellCoords.second < fieldYPos_start || cellCoords.second > fieldYPos_end)
                    {
                        std::cout << "---   Cell position coords wrong cellCoords.first: " << cellCoords.first << " cellCoords.second: " << cellCoords.second << std::endl;
                        continue;
                    }

                    // Анимированные клетки
                    std::shared_ptr<Cell>cell(new Cell(cellCoords.first, cellCoords.second, cellXSize, cellYSize, renderer));
                    cell->setInfo(x, y, "cell");

                    cell->setSprites("D:/Programming/Sobes/GameOfLife/GOL_SDL2/GameOfLife_SDL2/Res/cells.png", 5, 4, 40, 40, 0);
                    // cell->applyToRender();

                    // std::cout << "Field::showCells apply ptr" << std::endl;
                    cellsMap[x][y] = cell;

                    // std::cout << "x = " << x << " y = " << y << std::endl;
                }
            }
        }
    }


    // Проверка удалённых клеток + обновление поля
    lifeCells = 0;
    cellsToDelete.clear();

    // std::cout << "- Field::showCells cellsMap.size: " << cellsMap.size() << std::endl;
    for(auto xCell = cellsMap.begin(); xCell != cellsMap.end(); xCell++)
    {
        if(xCell->second.empty()) continue;
        // std::cout << "- Field::showCells xCell.second.size: " << xCell->second.size() << std::endl;
        for(auto yCell = xCell->second.begin(); yCell != xCell->second.end(); yCell++)
        {
            x = xCell->first;
            y = yCell->first;

            // std::cout << "-- Field::showCells x: " << x << " y: " << y << std::endl;

            // yCell->second->applyToRender();
            if(_cells.find(x) != _cells.end() && _cells.at(x).count(y))
            {
                lifeCells++;
            }
            else
            {
                if(yCell->second->isDead)
                {
                    // std::cout << "Field::showCells isDead delete cell x: " << x << " y: " << y << std::endl;

                    cellsToDelete[x].insert(y); /// <<< ---------------------------------------------------------------- Создаётся список клеток на удаление и удаляется в отдельном циклеcontinue;
                    // std::cout << "Field::showCells continue" << std::endl;
                }
                else
                {
                    // std::cout << "Field::showCells lifeCells is not life" << std::endl;
                    cellsMap[x][y]->isLife = false;
                }

                // std::cout << "Field::showCells deleted" << std::endl;
            }

            // std::cout << "Field::showCells applyToRender x: " << x << " y: " << y << std::endl;
            yCell->second->applyToRender();
        }
    }

    // Удаление мёртвых клеток
    for(auto xCell = cellsToDelete.begin(); xCell != cellsToDelete.end(); xCell++)
    {
        for(auto yCell = xCell->second.begin(); yCell != xCell->second.end(); yCell++)
        {
            cellsMap[xCell->first].erase(*yCell);
        }

        if(cellsMap[xCell->first].empty())
        {
            std::cout << "Field::showCells isDead cellsMap[x].empty: " << x << std::endl;
            cellsMap.erase(xCell->first);
            // std::cout << "Field::showCells break" << std::endl;
            // break;
        }
    }

    // std::cout << "Field::showCells lifeCells: " << lifeCells << std::endl;
}

int Field::getLifeCellsNum()
{
    return lifeCells;
}

bool Field::isMapUpdated(const std::map<int, std::set<int>> &_cells)
{
    std::cout << "Field::isMapUpdated _cells.size(): " << _cells.size() << std::endl;
    int x;
    int y;
    for(auto xCell = _cells.begin(); xCell != _cells.end(); xCell++)
    {
        for(auto yCell = xCell->second.begin(); yCell != xCell->second.end(); yCell++)
        {
            x = xCell->first;
            y = *yCell;
            if(cellsMap.find(x) == cellsMap.end() || (cellsMap.find(x) != cellsMap.end() && !cellsMap.at(x).count(y)))
            {
                std::cout << "Field::isMapUpdated 1 TRUE" << std::endl;
                return true;
            }
        }
    }

    for(auto xCell = cellsMap.begin(); xCell != cellsMap.end(); xCell++)
    {
        for(auto yCell = xCell->second.begin(); yCell != xCell->second.end(); yCell++)
        {
            x = xCell->first;
            y = yCell->first;
            if(_cells.find(x) == _cells.end() || (_cells.find(x) != _cells.end() && !_cells.at(x).count(y)))
            {
                std::cout << "Field::isMapUpdated 2 TRUE" << std::endl;
                return true;
            }
        }
    }

    return false;
}

void Field::fillCellsCoords()
{
    // std::cout << "Field::fillCellsCoords " << std::endl;
    cellsCoords.reserve(fieldXCells);
    for(int x = 0; x < fieldXCells; x++)
    {
        // std::cout << "Field::fillCellsCoords x: " << x <<  std::endl;
        std::vector<std::pair<int, int>> vec;
        vec.reserve(fieldYCells);
        for(int y = 0; y < fieldYCells; y++)
        {
            // std::cout << "  Field::fillCellsCoords y: " << y << std::endl;
            vec.push_back(std::make_pair(fieldXPos_start + (cellXSize + spacerSize) * x, fieldYPos_start + (cellYSize + spacerSize) * y));
            // std::cout << "          x coords: " << fieldXPos_start + (cellXSize + spacerSize) * x << " y coords: " << fieldYPos_start + (cellYSize + spacerSize) * y << std::endl;
        }
        cellsCoords.push_back(vec);
        std::cout << std::endl;
    }
}
