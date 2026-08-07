#ifndef STACK_H
#define STACK_H

#include <vector>
#include <cstddef>

class Stack
{
private:
    std::vector<int>stackMem;
public:
    Stack(size_t stackSize = 100)  {stackMem.assign(stackSize, 0);}

    void PushStack(const int &val);
    int PopStack ();
};

#endif