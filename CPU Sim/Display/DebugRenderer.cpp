#include "DebugRenderer.h"

void RenderDebug::OutputFrameBufferMemory (const std::vector<uint8_t> &memory, SHORT posX, SHORT posY){
    size_t count = memory.size();

    SHORT width = static_cast<SHORT>(std::sqrt(static_cast<double>(count))), height = width;
    if(static_cast<size_t>(width * height) != count) throw std::runtime_error("Framebuffer size must be a perfect square.");

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CHAR_INFO charBuffer[count];

    for(u_int i = 0; i < static_cast<int>(count); ++i){
        charBuffer[i].Char.AsciiChar = memory[i];
        charBuffer[i].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN; ///as ASCII characters
    }

    COORD bufferSize = {width, height};
    COORD bufferCoord = {0, 0};
    SMALL_RECT output = {posX, posY, (SHORT)(posX + width - 1), (SHORT)(posY + height - 1)};

    if(!WriteConsoleOutputA(hConsole, charBuffer, bufferSize, bufferCoord, &output)) throw std::runtime_error("Error: WriteConsoleOutputA failed.");
}

void RenderDebug::OutputFrameBufferOther(const std::string &data, SHORT posX, SHORT posY){
    size_t stringLength = data.length();
    size_t maxLength = 0;
    if(stringLength > maxLength) maxLength = stringLength;

    SHORT width = static_cast<SHORT>(maxLength), height = 1;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CHAR_INFO charBuffer[maxLength];

    for(size_t i = 0; i < maxLength; ++i){
        charBuffer[i].Char.AsciiChar = data[i];
        charBuffer[i].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    }

    COORD bufferSize = {width, height};
    COORD bufferCoord = {0, 0};

    SMALL_RECT dataOutput = {posX, posY, (SHORT)(posX + width - 1), (SHORT)(posY + height - 1)};

    if(!WriteConsoleOutputA(hConsole, charBuffer, bufferSize, bufferCoord, &dataOutput)) throw std::runtime_error("Error: WriteConsoleOutputA failed.");
}

std::string RenderDebug::PadString(const std::string &input, size_t width){
    if(input.length() >= width) return input.substr(0, width);
    return input + std::string(width - input.length(), ' ');
}