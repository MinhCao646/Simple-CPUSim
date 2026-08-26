#ifndef RENDER_H
#define RENDER_H

#include <vector>
#include <iostream>
#include <windows.h>
#include <cstdint>

struct ScreenPixel
{
    char symbol = '\0';
    wchar_t Usymbol = '\0';
    int color = 1;
};


class Render{
private:
    size_t height, width;
    std::vector<std::vector<ScreenPixel>> displayMemory;
public:
    Render(size_t height = 10, size_t width = 20) : height(height), width(width) {displayMemory.assign(height, std::vector<ScreenPixel>(width));}

    size_t GetScreenY                ();
    size_t GetScreenX                ();
    void OutputFrameBufferUnicode (SHORT posX, SHORT posY);
    void OutputFrameBuffer        (SHORT posX, SHORT posY);
    void AssignPixel              (size_t *y_axis, size_t *x_axis, char &symbol, uint8_t &color);
    void AssignPixelForUnicode    (size_t *y_axis, size_t *x_axis, wchar_t &symbol, uint8_t &color);
    void ResetPixelValue          ();
};

#endif