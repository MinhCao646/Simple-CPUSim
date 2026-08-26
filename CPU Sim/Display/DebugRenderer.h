#ifndef DBG_RENDER_H
#define DBG_RENDER_H

//#include "../Core/CPUCore.h"
#include <vector>
#include <iostream>
#include <windows.h>
#include <cmath>
#include <cstdint>

class RenderDebug
{
private:
    /* data */
public:
    void OutputFrameBufferMemory (const std::vector<uint8_t> &memory, SHORT posX, SHORT posY);
    void OutputFrameBufferOther  (const std::string &data, SHORT posX, SHORT posY);
    std::string PadString        (const std::string &input, size_t width);
};

#endif