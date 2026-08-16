#include "Renderer.h"

int Render::GetScreenY(){
    return height;
}

int Render::GetScreenX(){
    return width;
}

void Render::OutputFrameBufferUnicode(){
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
    SMALL_RECT writeRegion = {10, 5, static_cast<SHORT>(10 + width - 1), static_cast<SHORT>(5 + height - 1)};

    if(!WriteConsoleOutputW(hConsole, charBuffer, bufferSize, bufferCoord, &writeRegion)){
        throw std::runtime_error("Error: WriteConsoleOutputA failed.");
    }
}

void Render::OutputFrameBuffer(){
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
    SMALL_RECT writeRegion = {10, 5, static_cast<SHORT>(10 + width - 1), static_cast<SHORT>(5 + height - 1)};

    if(!WriteConsoleOutput(hConsole, charBuffer, bufferSize, bufferCoord, &writeRegion)){
        throw std::runtime_error("Error: WriteConsoleOutputA failed.");
    }
}

void Render::AssignPixel(int *y_axis, int *x_axis, char &symbol, int &color){
    displayMemory[*y_axis][*x_axis].color = color;
    displayMemory[*y_axis][*x_axis].symbol = symbol;
}

void Render::AssignPixelForUnicode    (int *y_axis, int *x_axis, wchar_t &symbol, int &color){
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