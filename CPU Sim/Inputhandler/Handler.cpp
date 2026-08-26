#include "Handler.h"

uint8_t GetInput(){
    if(kbhit()){
        char c = getch();
        return static_cast<uint8_t>(c);
    }
    return 0;
}