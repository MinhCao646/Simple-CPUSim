#ifndef CORE_H
#define CORE_H

#include "InstructionFormat.h"

#include <vector>
#include <windows.h>
#include "../Display/Renderer.h"
#include "../Core/LoopState.h"
#include "../Miscs/StackCaller.h"
#include "../Miscs/FlagFormat.h"
#include "../Miscs/BitMaskFlag.h"
#include "../Inputhandler/Handler.h"

class CPU
{
public:
    uint16_t programCounter = 0;
    bool running, executed = false;
    size_t ScrHeight, ScrWidth;
    Render rend;
    Stack stack;
    BitMaskFlag bit;
    std::vector<uint8_t> memory;
    std::vector<Instruction> programMemory;

    const SHORT posScrX = 5, posScrY = 5;
    SHORT posRelative = 0;

    CPU(size_t memorySize, size_t scrHeight, size_t scrWidth) : memory(memorySize), running(true), ScrHeight(scrHeight), ScrWidth(scrWidth), rend(ScrHeight, ScrWidth) {}

    void LoadProgram      (const std::vector<Instruction> &program);
    Instruction Fetch     ();
    void DecodeAndExecute (const Instruction &instr);
    void RunCPU           ();
};


#endif