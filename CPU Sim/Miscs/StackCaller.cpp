#include "StackCaller.h"

void Stack::PushStack(const int &val){
    if(topStack >= stackMem.size()) throw std::runtime_error("Stack overflow!");
    stackMem.at(topStack) = val;
    topStack++;
}

int Stack::PopStack (){
    if(topStack == 0) throw std::runtime_error("Stack underflow!");
    --topStack;
    int val = stackMem.at(topStack);
    stackMem.at(topStack) = 0;
    return val;
}