#include <iostream>
#include <vector>

#include "elements.h"
#include "field.h"
#include "gol.h"

#define SDL_NOFRAME

int main(int argc, char* argv[])
{
    // Инициализирует выбранное оборудование для приложения, видео, звук и т.д.  SDL_INIT_EVERYTHINK - Инициализирует всё
    SDL_Init(SDL_INIT_VIDEO);
    // Инициализация SDL_image
    IMG_Init(IMG_INIT_PNG);
    // Инициализация SDL_ttf
    TTF_Init();

    // Инициализация окна, принимает (Имя окна, Положение окна X, Положение окна Y, Размер окна X, Размер окна Y, Дополнительные флаги). Положение окна можно указать в пикселях от начала координат
    const int screenWidth = 720;
    const int screenHeight = 550;
    bool running = true;
    SDL_Window *window = SDL_CreateWindow("FirstWindow", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, 0);

    SDL_SetWindowBordered(window, SDL_FALSE); // Убрать заголовок окна и рамку

    bool isStarted = false;
    std::map<int, std::set<int>> cells;

    std::shared_ptr<SDL_Renderer> renderer(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED), SDL_DestroyRenderer);

    // Поле отображения графики - Включает перетаскивание окна. Принимает: (окно в котором будет работать, выбор графического устройства(если несколько), ускорение графики). Для ускорения графики без видеокарты(программно) SDL_RENDERER_SOFTWARE

    Element interface(0, 0, 100, 60, renderer);
    interface.setColor(255,255,255,255);
    interface.setBgImage("D:/Programming/Sobes/GameOfLife/GOL_SDL2/GameOfLife_SDL2/Res/Background.png");
    interface.setName("interfase");

    Element buttonStart(497, 75, 200, 50, renderer);
    buttonStart.setBgImage("D:/Programming/Sobes/GameOfLife/GOL_SDL2/GameOfLife_SDL2/Res/Button.png");
    buttonStart.setBgImageHovered("D:/Programming/Sobes/GameOfLife/GOL_SDL2/GameOfLife_SDL2/Res/Button_hovered.png");
    buttonStart.setBgImagePressed("D:/Programming/Sobes/GameOfLife/GOL_SDL2/GameOfLife_SDL2/Res/Button_pressed.png");
    buttonStart.setFont("D:/Programming/Sobes/GameOfLife/GOL_SDL2/GameOfLife_SDL2/Res/MISTRAL.TTF", 35);
    buttonStart.setTextColor(85, 60, 40);
    buttonStart.setText("Старт");
    buttonStart.setName("buttonStart");

    Element buttonNew(497, 145, 200, 50, renderer);
    buttonNew.setBgImage("D:/Programming/Sobes/GameOfLife/GOL_SDL2/GameOfLife_SDL2/Res/Button.png");
    buttonNew.setBgImageHovered("D:/Programming/Sobes/GameOfLife/GOL_SDL2/GameOfLife_SDL2/Res/Button_hovered.png");
    buttonNew.setBgImagePressed("D:/Programming/Sobes/GameOfLife/GOL_SDL2/GameOfLife_SDL2/Res/Button_pressed.png");
    buttonNew.setFont("D:/Programming/Sobes/GameOfLife/GOL_SDL2/GameOfLife_SDL2/Res/MISTRAL.TTF", 35);
    buttonNew.setTextColor(85, 60, 40);
    buttonNew.setText("Очистить");
    buttonNew.setName("buttonNew");

    Element buttonExit(497, 480, 200, 50, renderer);
    buttonExit.setBgImage("D:/Programming/Sobes/GameOfLife/GOL_SDL2/GameOfLife_SDL2/Res/Button.png");
    buttonExit.setBgImageHovered("D:/Programming/Sobes/GameOfLife/GOL_SDL2/GameOfLife_SDL2/Res/Button_hovered.png");
    buttonExit.setBgImagePressed("D:/Programming/Sobes/GameOfLife/GOL_SDL2/GameOfLife_SDL2/Res/Button_pressed.png");
    buttonExit.setFont("D:/Programming/Sobes/GameOfLife/GOL_SDL2/GameOfLife_SDL2/Res/MISTRAL.TTF", 35);
    buttonExit.setTextColor(85, 60, 40);
    buttonExit.setText("Выход");
    buttonExit.setName("buttonExit");

    Text textInfo(598, 225, renderer);
    textInfo.setFont("D:/Programming/Sobes/GameOfLife/GOL_SDL2/GameOfLife_SDL2/Res/Inkfree.ttf", 19);
    textInfo.setFontColor({85, 60, 40, 255});
    textInfo.setText("Расставить клетки");
    textInfo.setTextCentered(true);

    Text textCellsOnField(508, 245, renderer);
    textCellsOnField.setFont("D:/Programming/Sobes/GameOfLife/GOL_SDL2/GameOfLife_SDL2/Res/Inkfree.ttf", 18);
    textCellsOnField.setFontColor({85, 60, 40, 255});
    textCellsOnField.setText("Клеток на поле: ");

    Text textCellsOnFieldNum(690, 245, renderer);
    textCellsOnFieldNum.setFont("D:/Programming/Sobes/GameOfLife/GOL_SDL2/GameOfLife_SDL2/Res/Inkfree.ttf", 18);
    textCellsOnFieldNum.setFontColor({85, 60, 40, 255});
    textCellsOnFieldNum.setText("");
    textCellsOnFieldNum.setTextRight(true);

    Field field(renderer);

    // Движок GOL
    GOL gol(10, 10);

    // SDL_Rect rect = {25, 80, 445, 445};

    // Переменная содержит события нажатия клавиш
    SDL_Event event;

    // Главный цикл
    int mouseX;
    int mouseY;

    // Переменные для GOL
    int golDalay = 60;
    int golDalayCounter = 0;
    int colonySteps = 0;

    bool buttonStartStarted = false;

    while (running)
    {
        try {

            // Проверка наличия событий в очереди
            while (SDL_PollEvent(&event))
            {
                switch (event.type) {
                case SDL_QUIT: // Закрыть приложение
                    running = false;
                    break;

                case SDL_KEYDOWN:

                    break;

                case SDL_MOUSEBUTTONDOWN:
                    std::cout << "SDL_MOUSEBUTTONDOWN ";

                    if(event.button.button == SDL_BUTTON_LEFT)
                    {
                        if(buttonStart.pressed(event.button.x, event.button.y, true))
                        {
                            std::cout << "buttonStart pressed" << std::endl;
                            if(isStarted || buttonStartStarted)
                            {
                                buttonStart.setText("Старт");
                                isStarted = false;
                                buttonStartStarted = false;
                                cells.clear();
                            }
                            else
                            {
                                // gol.setCells(cells);
                                buttonStart.setText("Стоп");
                                isStarted = true;
                                buttonStartStarted = true;
                            }
                        }
                        if(buttonNew.pressed(event.button.x, event.button.y, true))
                        {
                            if(!isStarted)
                            {
                                cells.clear();
                                gol.setCells(cells);
                                std::cout << "buttonNew pressed" << std::endl;
                            }
                        }
                        if(buttonExit.pressed(event.button.x, event.button.y, true)) // Закрыть приложение
                        {
                            running = false;
                        }

                        // Клик по полю если не запущена GOL
                        if(!isStarted && field.isFieldInFocus(event.button.x, event.button.y))
                        {
                            std::pair<int, int> cell = field.getCellNum(event.button.x, event.button.y);
                            if(cells.find(cell.first) != cells.end() && cells.at(cell.first).count(cell.second))
                            {
                                cells[cell.first].erase(cell.second);
                                if(cells[cell.first].empty())
                                {
                                    cells.erase(cell.first);
                                }
                            }
                            else
                            {
                                cells[cell.first].insert(cell.second);
                            }

                            gol.setCells(cells);
                        }
                    }
                    else
                    {

                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    std::cout << "SDL_MOUSEBUTTONUP ";
                    if(event.button.button == SDL_BUTTON_LEFT)
                    {
                        buttonStart.pressed(event.button.x, event.button.y, false);
                        buttonNew.pressed(event.button.x, event.button.y, false);
                        buttonExit.pressed(event.button.x, event.button.y, false);
                    }
                    else
                    {

                    }
                    break;

                case SDL_MOUSEMOTION:
                    mouseX = event.motion.x;
                    mouseY = event.motion.y;

                    buttonStart.hovered(mouseX, mouseY);
                    buttonNew.hovered(mouseX, mouseY);
                    buttonExit.hovered(mouseX, mouseY);

                    break;
                default:
                    break;
                }
            }

            // Выбрать указанному рендереру цвет фона в формате RGB + Альфа канал
            SDL_SetRenderDrawColor(renderer.get(), 150, 255, 100, 255);
            // Очистить renderer и загрузить заново
            SDL_RenderClear(renderer.get());

            interface.applyToRender();

            // Получение текущего расположения клеток от GOL


            // Поле
            // field.showCells(gol.getCells());

            // Кнопки
            buttonStart.applyToRender();
            buttonNew.applyToRender();
            buttonExit.applyToRender();

            // Информационный текст
            textInfo.applyToRender();

            // textCellsOnField.setText("Клеток на поле: " );
            textCellsOnField.applyToRender();

            // textCellsOnFieldNum.setText(std::to_string(field.getLifeCellsNum()));
            // field.getLifeCellsNum();
            // std::string cellsNum(std::to_string(field.getLifeCellsNum()));
            textCellsOnFieldNum.setText(std::to_string(field.getLifeCellsNum()));
            textCellsOnFieldNum.applyToRender();

            // SDL_SetRenderDrawColor(renderer, 30, 50, 200, 100);
            // SDL_RenderFillRect(renderer, &rect);

            // GOL
            if(isStarted)
            {
                std::cout << ".";
                if(golDalayCounter < golDalay)
                {
                    golDalayCounter++;
                }
                else
                {
                    golDalayCounter = 0;
                    // cells = gol.getCells();

                    gol.startCells();
                    gol.printCellsCoords(gol.getCells());
                }

                if(gol.getCellsNum() == 0)
                {
                    isStarted = false;
                    colonySteps = 0;
                }
                else
                {
                    colonySteps++;
                }
            }

            cells = gol.getCells();

            // Вывод на экран(Получение текущего расположения клеток от GOL)
            field.showCells(cells);

            // Отображает содержимое рендерара
            SDL_RenderPresent(renderer.get());

            if(gol.getCellsNum() == 0)
            {
                buttonStart.setText("Старт");
                isStarted = false;
                buttonStartStarted = false;
                cells.clear();
            }

            // std::cout << std::endl;
            // Задержка 16 для получения 60 кадров в секунду. Надо учитывать время выполнения кода?
            SDL_Delay(16);
        } catch (...) {
            std::cout << "!!!   ERROR IN LOOP   !!!" << std::endl;
        }
    }

    // !!! Обязательно
    // Уничтожение окна перед закрытием приложения
    SDL_DestroyWindow(window);
    // Уничтожение рендерера перед закрытием приложения
    SDL_DestroyRenderer(renderer.get());

    // delete renderer;

    // Закрытие SDL
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}
