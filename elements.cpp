#include "elements.h"

Element::Element(std::shared_ptr<SDL_Renderer> &_renderer) : renderer(_renderer)
{
    rect = {xPos, yPos, width, height};
    hoverColor = {(uint8_t)(r + hoverColorParam), (uint8_t)(g + hoverColorParam), (uint8_t)(b + hoverColorParam), a};
}

Element::Element(const int &_xPos, const int &_yPos, const int &_width, const int &_height, std::shared_ptr<SDL_Renderer> &_renderer) : Element(_renderer)
{
    setPos(_xPos, _yPos);
    setSize(_width, _height);
}

Element::~Element()
{
    std::cout << "Element::~Element name: " << name << std::endl;

    std::cout << "Element::~Element font" << std::endl;
        // SDL_Destroy
    TTF_CloseFont(font);

    std::cout << "Element::~Element textSurface" << std::endl;
    SDL_FreeSurface(textSurface);

    std::cout << "Element::~Element textTexture" << std::endl;
    SDL_DestroyTexture(textTexture);
    // delete textTexture;

    std::cout << "Element::~Element hoverFont text: " << text << std::endl;
    TTF_CloseFont(hoverFont);

    std::cout << "Element::~Element bgTex" << std::endl;
    SDL_DestroyTexture(bgTex);

    std::cout << "Element::~Element bgTexHovered" << std::endl;
    SDL_DestroyTexture(bgTexHovered);
    // delete bgTexHovered;

    std::cout << "Element::~Element bgTexPressed" << std::endl;
    SDL_DestroyTexture(bgTexPressed);
    // delete bgTexPressed;

    // SDL_Rect_Free(&rect);

    ///--------------------------------------------------------------------
    // try
    // {


    // } catch (...)
    // {
    //     std::cout << "LOL !!!" << std::endl;
    // }

    // if(font != nullptr)
    // {
    //     std::cout << "Element::~Element font" << std::endl;
    //         // SDL_Destroy
    //     TTF_CloseFont(font);
    // }

    // if(textSurface != nullptr)
    // {
    //     std::cout << "Element::~Element textSurface" << std::endl;
    //     SDL_FreeSurface(textSurface);
    // }

    // if(textTexture != nullptr)
    // {
    //     std::cout << "Element::~Element textTexture" << std::endl;
    //     SDL_DestroyTexture(textTexture);
    //     // delete textTexture;
    // }

    // if(hoverFont != nullptr)
    // {
    //     std::cout << "Element::~Element hoverFont text: " << text << std::endl;
    //     TTF_CloseFont(hoverFont);
    // }

    // if(bgTex != nullptr)
    // {
    //     std::cout << "Element::~Element bgTex" << std::endl;
    //     SDL_DestroyTexture(bgTex);
    // }

    // if(bgTex != nullptr)
    // {
    //     std::cout << "Element::~Element bgTexHovered" << std::endl;
    //     SDL_DestroyTexture(bgTexHovered);
    //     // delete bgTexHovered;
    // }

    // if(bgTex != nullptr)
    // {
    //     std::cout << "Element::~Element bgTexPressed" << std::endl;
    //     SDL_DestroyTexture(bgTexPressed);
    //     // delete bgTexPressed;
    // }
    std::cout << "Element::~Element DONE" << std::endl;
}

void Element::setPos(const int &_xPos, const int &_yPos)
{
    xPos = _xPos;
    yPos = _yPos;
    rect.x = _xPos;
    rect.y = _yPos;
}

void Element::setSize(const int &_width, const int &_height)
{
    width = _width;
    height = _height;
    rect.w = _width;
    rect.h = _height;
}

void Element::setName(const std::string &_name)
{
    name = _name;
}

void Element::setColor(const uint8_t &_r, const uint8_t &_g, const uint8_t &_b, const uint8_t &_a)
{
    r = _r;
    g = _g;
    b = _b;
    a = _a;
    haveColor = true;
}

void Element::setText(const std::string &_text)
{
    text = _text;

    textSurface = TTF_RenderUTF8_Blended(font, text.c_str(), {fontR, fontG, fontB, fontA});
    textTexture = SDL_CreateTextureFromSurface(renderer.get(), textSurface);
    SDL_QueryTexture(textTexture, NULL, NULL, &fontW, &fontH);
    textRect = {rect.x + (rect.w - fontW) / 2, rect.y + (rect.h - fontH) / 2, fontW, fontH};
}

void Element::setFont(const std::string &_fontPath, const int &_size)
{
    font = TTF_OpenFont(_fontPath.c_str(), _size);
}

void Element::setTextColor(const uint8_t &_r, const uint8_t &_g, const uint8_t &_b, const uint8_t &_a)
{
    fontR = _r;
    fontG = _g;
    fontB = _b;
    fontA = _a;
}

void Element::setBgImage(const std::string &_imgPath)
{
    bgTex = IMG_LoadTexture(renderer.get(), _imgPath.c_str());
    SDL_QueryTexture(bgTex, NULL, NULL, &bgTexW, &bgTexH);
}

void Element::setBgImageHovered(const std::string &_imgPath)
{
    bgTexHovered = IMG_LoadTexture(renderer.get(), _imgPath.c_str());
}

void Element::setBgImagePressed(const std::string &_imgPath)
{
    bgTexPressed = IMG_LoadTexture(renderer.get(), _imgPath.c_str());
}

void Element::applyToRender()
{
    if(renderer != nullptr && haveColor)
    {
        if(isHovered)
        {
            SDL_SetRenderDrawColor(renderer.get(), hoverColor.r, hoverColor.g, hoverColor.b, hoverColor. a);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer.get(), r, g, b, a);
        }
    }
    if(bgTex != nullptr)
    {
        rect = {xPos, yPos, bgTexW, bgTexH};
        if(isPressed && bgTexPressed != nullptr)
        {
            SDL_RenderCopy(renderer.get(), bgTexPressed, NULL, &rect);
        }
        else if(isHovered && bgTexHovered != nullptr)
        {
            SDL_RenderCopy(renderer.get(), bgTexHovered, NULL, &rect);
        }
        else
        {
            SDL_RenderCopy(renderer.get(), bgTex, NULL, &rect);
        }
    }
    else
    {
        rect = {xPos, yPos, width, height};
        SDL_RenderFillRect(renderer.get(), &rect);
    }
    if(text.size() > 0)
    {
        SDL_RenderCopy(renderer.get(), textTexture, NULL, &textRect);
    }
}

bool Element::isAimed(const int &_x, const int &_y)
{
    return _x >= rect.x && _x <= rect.x + rect.w && _y >= rect.y && _y <= rect.y + rect.h;
}

bool Element::hovered(const int &_x, const int &_y)
{
    if(isAimed(_x, _y))
    {
        isHovered = true;
        return true;
    }

    isPressed = false;
    isHovered = false;
    return false;
}

bool Element::pressed(const int &_x, const int &_y, bool _pressed)
{
    if(_pressed && isAimed(_x, _y))
    {
        isPressed = true;
        return true;
    }

    isPressed = false;
    return false;
}

// -----------------------------------------------------------------------------------------------------------
/// ----------------------------------------------------------------------------------------------------------
Cell::Cell(const int &_xPos, const int &_yPos, const int &_width, const int &_height, std::shared_ptr<SDL_Renderer> &_renderer)
    : Element(_xPos, _yPos, _width, _height, _renderer)
{
    lastUpdate = SDL_GetTicks(); // Получаем количество времени (Тиков) прошедшее с момента запуска программы.
    isLife = true;
}

Cell::~Cell()
{
    std::cout << "Cell::~Cell name: " << name << " cellXPos: " << cellXPos << " cellYPos: " << cellYPos << std::endl;

    if(spritesTextures != nullptr)
    {
        std::cout << "Cell::~Cell spritesTextures" << std::endl;
        SDL_DestroyTexture(spritesTextures);
    }
    // Element::~Element();
    std::cout << "Cell::~Cell DONE" << std::endl;
}

void Cell::setSprites(const std::string &_path, const int &_spritesXCount, const int &_spritesYCount, const int &_spriteXSize, const int &_spriteYSize, const int &_spriteLine)
{
    spritesXCount = _spritesXCount;
    spritesYCount = _spritesYCount;
    spriteXSize = _spriteXSize;
    spriteYSize = _spriteYSize;
    srand(static_cast<unsigned int>(std::time(nullptr)));
    spriteLine = _spriteXSize * (std::rand() %4);

    currentSprite = 0;

    spritesTextures = IMG_LoadTexture(renderer.get(), _path.c_str());
}

void Cell::setInfo(const int &_cellXPos, const int &_cellYPos, const std::string &_name)
{
    cellXPos = _cellXPos;
    cellYPos = _cellYPos;
    name = _name;
}

void Cell::applyToRender()
{
    if(SDL_GetTicks() - lastUpdate > 50 && !isDead)
    {
        if(currentSprite < 4) // Появление || исчезание
        {
            currentSprite++;
        }
        else if(!isLife && currentSprite < 9)
        {
            currentSprite++;
        }
        else if(isLife && currentSprite > 4)
        {
            currentSprite--;
        }

        if(currentSprite == 9 && !isLife)
        {
            isDead = true;
            return;
        }

        lastUpdate = SDL_GetTicks();
    }

    spriteRectSrc = {currentSprite * spriteXSize, spriteLine, spriteXSize, spriteYSize};
    /// !!! в качестве прямоугольника dest для отрисовки текстуры используется rect класса Element
    rect = {xPos, yPos, spriteXSize, spriteYSize};


    if(SDL_GetTicks() - lastUpdate > animationSpeed)
    {
        lastUpdate = SDL_GetTicks();
    }

    // Element::applyToRender();
    SDL_RenderCopy(renderer.get(), spritesTextures, &spriteRectSrc, &rect);
}

Text::Text(const int &_xPos, const int &_yPos, std::shared_ptr<SDL_Renderer> &_renderer) : renderer(_renderer)
{
    isChanged = true;

    xPos = _xPos;
    yPos = _yPos;
}

void Text::setFont(const std::string &_fontPath, const int &_fontSize)
{
    isChanged = true;
    font = TTF_OpenFont(_fontPath.c_str(), _fontSize);
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
}

void Text::setFontColor(const SDL_Color &_fontColor)
{
    isChanged = true;
    fontColor = _fontColor;
}

void Text::setText(const std::string _text)
{
    if(text != _text)
    {
        isChanged = true;
        text = _text;
    }
}

void Text::applyTexture()
{
    SDL_DestroyTexture(fontTex);

    fontSurface = TTF_RenderUTF8_Blended(font, text.c_str(), fontColor);
    fontTex = SDL_CreateTextureFromSurface(renderer.get(), fontSurface);

    // Получить размер текста
    SDL_QueryTexture(fontTex, NULL, NULL, &fontW, &fontH);
    // Прямоугольник для отображения текста
    if(isCentered)
    {
        fontRect = {xPos - fontW / 2, yPos, fontW, fontH};
    }
    else if(isRight)
    {
        fontRect = {xPos - fontW, yPos, fontW, fontH};
    }
    else
    {
        fontRect = {xPos, yPos, fontW, fontH}; // Чтобы растянуть текст, надо указать размеры вручную
    }
    isChanged = false;
}

void Text::applyToRender()
{
    if(isChanged)
    {
        applyTexture();
    }

    // Отображение текста в цикле
    SDL_RenderCopy(renderer.get(), fontTex, NULL, &fontRect);
}

void Text::setName(const std::string &_name)
{
    name = _name;
}

void Text::setTextCentered(bool _isCentered)
{
    isCentered = _isCentered;
}

void Text::setTextRight(bool _isRight)
{
    isRight = _isRight;
}
