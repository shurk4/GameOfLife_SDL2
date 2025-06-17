#ifndef ELEMENTS_H
#define ELEMENTS_H
#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <memory>

#include <iostream>
#include <string>
#include <chrono>

// struct CellsCoords
// {
//     CellsCoords() {}
//     int xStart;
//     int xEnd;
//     int yStart;
//     int yEnd;
// };

class Element //-
{
    friend class Cell;

    // Состояния
    // Основное
    bool isHovered = false;
    bool isPressed = false;

    // Rectangle область элемента
    std::string name = "Element";
    int winXSize = 100;
    int winYSize = 50;
    int xPos = 10;
    int yPos = 10;
    int width = 100;
    int height = 50;
    SDL_Rect rect;
    std::shared_ptr<SDL_Renderer> renderer;
    SDL_Texture *texture = nullptr;

    // Текст
    int fontW = 100;
    int fontH = 50;

    uint8_t fontR = 0;
    uint8_t fontG = 0;
    uint8_t fontB = 0;
    uint8_t fontA = 255;

    SDL_Rect textRect;
    std::string currentText = "";
    std::string text = "";
    TTF_Font *font = nullptr;
    SDL_Surface *textSurface = nullptr;
    SDL_Texture *textTexture = nullptr;
    // Цвет
    bool haveColor = false;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 255;

    // Наведено - isHovered = true;
    // Цвет
    uint8_t hoverColorParam = 50; // Меняет каждый градиент цвета на укзанное значение, допускается отрицательное. НЕТ ПРОВЕРКИ!
    SDL_Color hoverColor;
    TTF_Font *hoverFont;

    // Фоновое изображение
    SDL_Texture *bgTex = nullptr;
    int bgTexW = 10;
    int bgTexH = 10;
    // Фоновое наведено
    SDL_Texture *bgTexHovered = nullptr;

    // Фоновое нажато
    SDL_Texture *bgTexPressed = nullptr;

    // Текст
    // std::string hoverText = "";
    // SDL_Color hoverTextColor = {255, 255, 255, 255};

    // Перетаскивание
    bool draggable = false;

public:
    // Быстрый способ
    // Создать экземпляр класса с указанием позиций и рендера
    // Применить цвет
    // Выполнить applyToRender
    Element(std::shared_ptr<SDL_Renderer>&_renderer); //+
    Element(const int &_xPos, const int &_yPos, const int &_width, const int &_height, std::shared_ptr<SDL_Renderer>&_renderer); //+
    ~Element();

    // Устанавливает размеры родительского окна. Нужно для позиционирования?
    void setWindowSize(const int &_x, const int &_y); //+
    void setPos(const int &_xPos, const int &_yPos);//+
    void setSize(const int &_width, const int &_height);//+

    void setColor(const uint8_t &_r = 0, const uint8_t &_g = 0, const uint8_t &_b = 0, const uint8_t &_a = 255); //+

    // void applyText();
    void setText(const std::string &_text);
    void setFont(const std::string &_fontPath, const int &_size);
    void setTextColor(const uint8_t &_r = 0, const uint8_t &_g = 0, const uint8_t &_b = 0, const uint8_t &_a = 255);

    void setBgImage(const std::string &_imgPath);
    void setBgImageHovered(const std::string &_imgPath);
    void setBgImagePressed(const std::string &_imgPath);

    // Методы для наведения
    bool isAimed(const int &_x, const int &_y);
    // Меняет состояние при наведении, принимает координаты мыши, помещать в event.type - SDL_MOUSEMOTION
    bool hovered(const int &_x, const int &_y); //+
    bool pressed(const int &_x, const int &_y , bool _pressed = false);
    // Установка шрифта для текста при наведении, принимает строку шрифт, r,g,b,a, size, text
    void setHoverText(const std::string &_fontPath, const int &_r = 255, const int &_g = 255, const int &_b = 255, const int &_a = 255, const int &_size = 14, const std::string &_hoverText = "");

    // Применяет все параметры и отрисовывает в рендер
    void applyToRender(); //+
private:

};

class Cell : public Element
{
    SDL_Texture *spritesTextures = nullptr;

    // Количество спрайтов по X и Y
    int spritesXCount = 5;
    int spritesYCount = 4;
    int spriteXSize = 40;
    int spriteYSize = 40;
    int spriteLine = 0;
    SDL_Rect spriteRectSrc;

    Uint32 animationSpeed = 100;
    Uint32 lastUpdate;
    int currentSprite = 0;

public:
    Cell(const int &_xPos, const int &_yPos, const int &_width, const int &_height, std::shared_ptr<SDL_Renderer>&_renderer);
    ~Cell();

    void setSprites(const std::string &_path, const int &_spritesXCount, const int &_spritesYCount, const int &_spriteXSize, const int &_spriteYSize, const int &_spriteLine = 0);
    bool isLife = false;
    bool isDead = false;

    void applyToRender();
};

class Text
{
    bool isChanged = false;
    bool isCentered = false;
    bool isRight = false;

    int xPos = 10;
    int yPos = 10;

    std::shared_ptr<SDL_Renderer> &renderer;
    TTF_Font *font = nullptr;
    SDL_Color fontColor = {0, 0, 0, 255};
    SDL_Surface *fontSurface = nullptr;
    SDL_Texture *fontTex = nullptr;
    SDL_Rect fontRect;

    std::string text = "";

    int fontW;
    int fontH;

public:
    Text(const int &_xPos, const int &_yPos, std::shared_ptr<SDL_Renderer> &_renderer);

    void setFont(const std::string &_fontPath, const int &_fontSize);
    void setFontColor(const SDL_Color &_fontColor);
    void setText(const std::string _text);
    void applyTexture();
    void applyToRender();

    void setTextCentered(bool _isCentered);
    void setTextRight(bool _isRight);
};

#endif // ELEMENTS_H
