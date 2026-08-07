#include "StackCaller.h"

void Stack::PushStack(const int &val){
    for(int i = stackMem.size() - 1; i > 0; --i){
        stackMem.at(i) = stackMem.at(i - 1);
    }
    stackMem.at(0) = val;
}

int Stack::PopStack (){
    int topStack = stackMem.at(0);
    for (int i = 0; i < stackMem.size() - 1; i++)
    {
        stackMem.at(i) = stackMem.at(i + 1);
    }
    return topStack;
}