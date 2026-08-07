#include "Handler.h"

int GetInput(){
    if(kbhit()){
        char c = getch();
        return static_cast<int>(c);
    }
}