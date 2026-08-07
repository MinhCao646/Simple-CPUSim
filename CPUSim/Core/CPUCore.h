#ifndef CORE_H
#define CORE_H

#include "InstructionFormat.h"

#include <vector>
#include "../Display/Renderer.h"
#include "../Miscs/StackCaller.h"
#include "../Miscs/FlagFormat.h"
#include "../Miscs/BitMaskFlag.h"
#include "../Inputhandler/Handler.h"

class CPU
{
private:
    int programCounter = 0;
    bool running;
    Render rend;
    Stack stack;
    BitMaskFlag bit;
    std::vector<int> memory;
    std::vector<Instruction> programMemory;
public:
    CPU(size_t memorySize = 256) : memory(memorySize), running(true) {}

    void LoadProgram      (const std::vector<Instruction> &program);
    Instruction Fetch     ();
    void DecodeAndExecute (const Instruction &instr);
    void RunCPU           ();
};


#endif