#ifndef STACK_H
#define STACK_H

#include <vector>
#include <cstddef>
#include <stdexcept>

class Stack
{
private:
    std::vector<int>stackMem;
    size_t topStack = 0;
public:
    Stack(size_t stackSize = 100)  {stackMem.assign(stackSize, 0);}

    void PushStack(const int &val);
    int PopStack ();
};

#endif