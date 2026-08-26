#include "Renderer.h"

size_t Render::GetScreenY(){
    return height;
}

size_t Render::GetScreenX(){
    return width;
}

void Render::OutputFrameBufferUnicode(SHORT posX, SHORT posY){
    int height = displayMemory.size();
    int width = displayMemory.at(0).size();

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if(!SetConsoleOutputCP(CP_UTF8)) throw std::runtime_error("\nError: Unable to set UTF-8 output code page.");
    CHAR_INFO charBuffer[width * height];

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = y * width + x;
            charBuffer[index].Char.UnicodeChar = displayMemory[y][x].Usymbol;
            charBuffer[index].Attributes = displayMemory[y][x].color;
        }
        
    }
    
    COORD bufferSize = {(SHORT)width, (SHORT)height};
    COORD bufferCoord = {0, 0};
    SMALL_RECT writeRegion = {posX, posY, static_cast<SHORT>(posX + width - 1), static_cast<SHORT>(posY + height - 1)};

    if(!WriteConsoleOutputW(hConsole, charBuffer, bufferSize, bufferCoord, &writeRegion)){
        throw std::runtime_error("Error: WriteConsoleOutputW failed.");
    }
}

void Render::OutputFrameBuffer(SHORT posX, SHORT posY){
    int height = displayMemory.size();
    int width = displayMemory.at(0).size();

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CHAR_INFO charBuffer[width * height];

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = y * width + x;
            charBuffer[index].Char.AsciiChar = displayMemory[y][x].symbol;
            charBuffer[index].Attributes = displayMemory[y][x].color;
        }
        
    }
    
    COORD bufferSize = {(SHORT)width, (SHORT)height};
    COORD bufferCoord = {0, 0};
    SMALL_RECT writeRegion = {posX, posY, static_cast<SHORT>(posX + width - 1), static_cast<SHORT>(posY + height - 1)};

    if(!WriteConsoleOutput(hConsole, charBuffer, bufferSize, bufferCoord, &writeRegion)){
        throw std::runtime_error("Error: WriteConsoleOutputA failed.");
    }
}

void Render::AssignPixel(size_t *y_axis, size_t *x_axis, char &symbol, uint8_t &color){
    displayMemory[*y_axis][*x_axis].color = color;
    displayMemory[*y_axis][*x_axis].symbol = symbol;
}

void Render::AssignPixelForUnicode(size_t *y_axis, size_t *x_axis, wchar_t &symbol, uint8_t &color){
    displayMemory[*y_axis][*x_axis].color = color;
    displayMemory[*y_axis][*x_axis].Usymbol = symbol;
}

void Render::ResetPixelValue(){
    for(auto &row : displayMemory){
        for(auto &pixel : row){
            pixel.color = 0;
            pixel.symbol = '\0';
            pixel.Usymbol = '\0';
        }
    }
}