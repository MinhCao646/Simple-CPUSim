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
    int ScrHeight, ScrWidth;
    Render rend;
    Stack stack;
    BitMaskFlag bit;
    std::vector<int> memory;
    std::vector<Instruction> programMemory;
public:
    CPU(size_t memorySize = 256, int scrHeight = 10, int scrWidth= 20) : memory(memorySize), running(true), ScrHeight(scrHeight), ScrWidth(scrWidth), rend(ScrHeight, ScrWidth) {}

    void LoadProgram      (const std::vector<Instruction> &program);
    Instruction Fetch     ();
    void DecodeAndExecute (const Instruction &instr);
    void RunCPU           ();
};


#endif
