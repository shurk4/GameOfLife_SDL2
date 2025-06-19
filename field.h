#ifndef FIELD_H
#define FIELD_H
#include <vector>
#include <map>
#include <set>
#include <memory>

#include "elements.h"

class Field
{
    int fieldXSize = 445;
    int fieldYSize = 445;

    int fieldXPos_start = 25;
    int fieldXPos_end = fieldXPos_start + fieldXSize;
    int fieldYPos_start = 80;
    int fieldYPos_end = fieldYPos_start + fieldYSize;

    int fieldXCells = 10;
    int fieldYCells = 10;

    int cellXSize = 40;
    int cellYSize = 40;
    int spacerSize = 5;

    int lifeCells = 0;

    std::vector<std::vector<std::pair<int, int>>> cellsCoords;
    std::map<int, std::set<int>> cellsToDelete;


    std::map<int, std::map<int, std::shared_ptr<Cell>>> cellsMap;
    std::vector<Cell*> newCells;

    void clearCellsVec(std::vector<Cell*> &_cells);

    std::shared_ptr<SDL_Renderer> renderer;

public:
    Field(std::shared_ptr<SDL_Renderer> &_renderer);

    bool isFieldInFocus(const int &_mouseX, const int &_mouseY);

    std::pair<int, int> getCellNum(const int &_mouseX, const int &_mouseY);
    std::pair<int, int> getCellCoords(const int &_x, const int &_y);

    void showCells(const std::map<int, std::set<int>> &_cells);
    int getLifeCellsNum();

private:
    bool isMapUpdated(const std::map<int, std::set<int>> &_cells);

    void fillCellsCoords();
};

#endif // FIELD_H
