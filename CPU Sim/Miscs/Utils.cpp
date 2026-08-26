#include "Utils.h"

void Delay (int miliseconds){
#ifdef WIN32
    Sleep(miliseconds);
#else
    usleep(miliseconds * 1000);
#endif
}

void ClearConsole(){
    #ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = {0, 0};
    if (hConsole == INVALID_HANDLE_VALUE) return;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;
    //Fill entire screen with spaces
    if (!FillConsoleOutputCharacter(hConsole, (TCHAR) ' ', cellCount, homeCoords, &count)) return;
    //Fill the entire screen with the current colors and attributes
    if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count)) return;
    //Move the cursor home
    SetConsoleCursorPosition(hConsole, homeCoords);
    #else
        std::cout << "\033[2J\033[H";
    #endif // _WIN32
}