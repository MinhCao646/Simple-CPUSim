#ifndef RENDER_H
#define RENDER_H

#include <vector>
#include <iostream>
#include <windows.h>

struct ScreenPixel
{
    char symbol = '\0';
    wchar_t Usymbol = '\0';
    int color = 1;
};


class Render{
private:
    int height, width;
    std::vector<std::vector<ScreenPixel>> displayMemory;
public:
    Render(int height = 10, int width = 20) : height(height), width(width) {displayMemory.assign(height, std::vector<ScreenPixel>(width));}

    int GetScreenY                ();
    int GetScreenX                ();
    void OutputFrameBufferUnicode ();
    void OutputFrameBuffer        ();
    void AssignPixel              (int *y_axis, int *x_axis, char &symbol, int &color);
    void AssignPixelForUnicode    (int *y_axis, int *x_axis, wchar_t &symbol, int &color);
    void ResetPixelValue          ();
};

#endif